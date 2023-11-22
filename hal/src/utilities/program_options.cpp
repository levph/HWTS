#include "hal_core/utilities/program_options.h"

#include "hal_core/utilities/log.h"
#include "hal_core/utilities/program_arguments.h"

#include <algorithm>
#include <unistd.h>

#if __linux__ || __APPLE__
#include <sys/ioctl.h>
#endif

namespace hal
{
    const std::string ProgramOptions::A_REQUIRED_PARAMETER = "__A_REQUIRED_PARAMETER__";

    ProgramOptions::ProgramOptions(const std::string& name)
    {
        m_name = name;
    }

    std::vector<std::string> ProgramOptions::get_unknown_arguments()
    {
        return m_unknown_options;
    }

    ProgramArguments ProgramOptions::parse(int argc, const char* argv[])
    {
        const Option* current_option = nullptr;

        m_unknown_options.clear();

        auto all_options = get_all_options();

        std::string current_flag;
        std::vector<std::string> current_parameters;
        size_t param_pos = 0;

        ProgramArguments args(argc, argv);

        // i=1: ignore first argument as it is the program itself
        for (int i = 1; i < argc; ++i)
        {
            std::string arg(argv[i]);

            // search through all options for a recognized flag
            bool opt_found = false;
            for (auto opt : all_options)
            {
                if (std::find(opt->flags.begin(), opt->flags.end(), arg) != opt->flags.end())
                {
                    // flag found, set current option

                    if (current_option != nullptr)
                    {
                        if (current_option->parameters[param_pos] == A_REQUIRED_PARAMETER)
                        {
                            die("core",
                                "the option with flags {}is missing a required parameter!",
                                utils::join(" ", std::vector<std::string>(current_option->flags.begin(), current_option->flags.end())));
                        }
                        else
                        {
                            args.set_option(current_flag, current_option->flags, current_parameters);
                        }
                    }

                    opt_found          = true;
                    current_option     = opt;
                    current_parameters = opt->parameters;
                    current_flag       = arg;
                    param_pos          = 0;
                    break;
                }
            }

            // if the current string is not a flag, process it otherwise
            if (!opt_found)
            {
                // if the current option expects parameters, add the current string
                if (current_option != nullptr)
                {
                    current_parameters[param_pos++] = arg;
                }
                else
                {
                    m_unknown_options.push_back(arg);
                }
            }

            // if the current option got all expected parameters, it is finished
            if (current_option != nullptr && param_pos >= current_option->parameters.size())
            {
                args.set_option(current_flag, current_option->flags, current_parameters);
                current_option = nullptr;
                current_parameters.clear();
                current_flag = "";
                param_pos    = 0;
            }
        }

        if (current_option != nullptr)
        {
            if (param_pos < current_option->parameters.size() && current_option->parameters[param_pos] == A_REQUIRED_PARAMETER)
            {
                die("core",
                    "the option with flags {}is missing at least one required parameter!",
                    utils::join(" ", std::vector<std::string>(current_option->flags.begin(), current_option->flags.end())));
            }

            args.set_option(current_flag, current_option->flags, current_parameters);
        }

        return args;
    }

    bool ProgramOptions::is_registered(const std::string& flag) const
    {
        for (auto opt : get_all_options())
        {
            if (std::find(opt->flags.begin(), opt->flags.end(), flag) != opt->flags.end())
            {
                return true;
            }
        }
        return false;
    }

    bool ProgramOptions::add(const std::string& flag, const std::string& description, const std::initializer_list<std::string>& parameters)
    {
        return add({flag}, description, parameters);
    }

    bool ProgramOptions::add(const std::initializer_list<std::string>& flags, const std::string& description, const std::initializer_list<std::string>& parameters)
    {
        if (flags.size() == 0)
        {
            log_error("core", "can't add option with empty flags (description: '{}')!", utils::trim(description));
            return false;
        }

        if (description.empty())
        {
            std::string flags_string = "";
            for (const auto& flag : flags)
            {
                flags_string += flag + " ";
            }

            log_error("core", "can't add option with flags ({}). The description must not be empty!", utils::trim(flags_string));
            return false;
        }

        // look for already registered option, abort if found
        for (auto opt : get_all_options())
        {
            if (opt->description == description)
            {
                std::string flags_string = "";
                for (const auto& flag : flags)
                {
                    flags_string += flag + " ";
                }

                log_error("core", "can't add option with flags ({}). An option with description '{}' is already registered.", utils::trim(flags_string), description);
                return false;
            }
        }

        for (const auto& flag : flags)
        {
            if (is_registered(flag))
            {
                log_error("core", "the flag '{}' is already registered.", flag);
                return false;
            }
        }

        // only allow REQUIRED, REQUIRED, REQUIRED, [...], NON_REQUIRED, NON_REQUIRED, [...]
        bool found_not_required = false;
        for (const auto& param : parameters)
        {
            if (param == A_REQUIRED_PARAMETER)
            {
                if (found_not_required)
                {
                    log_error("core", "a required parameter can't follow a non-required parameter.");
                    return false;
                }
            }
            else if (param != A_REQUIRED_PARAMETER)
            {
                found_not_required = true;
            }
        }

        // create new option
        Option opt;
        opt.description = description;
        opt.parameters  = parameters;
        opt.flags       = flags;

        m_options.push_back(opt);

        return true;
    }

    bool ProgramOptions::add(const ProgramOptions& other_options, const std::string& category)
    {
        // look for conflicts
        for (auto other_opt : other_options.get_all_options())
        {
            for (auto opt : get_all_options())
            {
                if (opt->description == other_opt->description)
                {
                    log_error("core", "an option with description '{}' is already registered.", opt->description);
                    return false;
                }

                for (const auto& flag : other_opt->flags)
                {
                    if (std::find(opt->flags.begin(), opt->flags.end(), flag) != opt->flags.end())
                    {
                        log_error("core", "the flag '{}' is already registered.", flag);
                        return false;
                    }
                }
            }
        }

        // no conflict -> add
        m_suboptions[category].push_back(other_options);
        return true;
    }

    bool ProgramOptions::remove(const std::string& flag)
    {
        for (auto it = m_options.begin(); it != m_options.end(); ++it)
        {
            if (it->flags.find(flag) != it->flags.end())
            {
                it->flags.erase(flag);
                if (it->flags.empty())
                {
                    m_options.erase(it);
                }
                return true;
            }
        }
        for (auto& sub : m_suboptions)
        {
            for (auto& opt : sub.second)
            {
                auto success = opt.remove(flag);
                if (success)
                {
                    return true;
                }
            }
        }
        return false;
    }

    std::string ProgramOptions::get_options_string() const
    {
        size_t line_width = 80;    // default magic number

#if __linux__ || __APPLE__
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if (w.ws_col < 30)
        {
            line_width = 1000;
        }
        else
        {
            line_width = w.ws_col;
        }
#endif

        return get_options_string_internal(get_flag_length() + 10, line_width);
    }

    std::vector<std::tuple<std::set<std::string>, std::string>> ProgramOptions::get_options() const
    {
        std::vector<std::tuple<std::set<std::string>, std::string>> options;
        for (auto opt : get_all_options())
        {
            options.push_back(std::make_tuple(opt->flags, opt->description));
        }
        return options;
    }

    std::vector<const ProgramOptions::Option*> ProgramOptions::get_all_options() const
    {
        std::vector<const ProgramOptions::Option*> options;

        std::transform(m_options.begin(), m_options.end(), std::back_inserter(options), [](auto& opt) { return &opt; });

        for (const auto& it : m_suboptions)
        {
            for (const auto& opt : it.second)
            {
                auto insertion = opt.get_all_options();
                options.insert(options.end(), insertion.begin(), insertion.end());
            }
        }
        return options;
    }

    size_t ProgramOptions::get_flag_length() const
    {
        size_t max_len = 0;
        for (const auto& opt : m_options)
        {
            size_t opt_len = 0;
            for (const auto& f : opt.flags)
            {
                opt_len += f.size() + 2;
            }
            for (size_t j = 0; j < opt.parameters.size(); ++j)
            {
                opt_len += 4;    //" arg"
            }
            opt_len -= 2;    // last option has no ", "

            if (opt_len > max_len)
            {
                max_len = opt_len;
            }
        }

        for (const auto& it : m_suboptions)
        {
            for (const auto& opt : it.second)
            {
                size_t subopt_len = opt.get_flag_length();
                if (subopt_len > max_len)
                {
                    max_len = subopt_len;
                }
            }
        }

        return max_len;
    }

    std::string ProgramOptions::get_options_string_internal(size_t fill_length, size_t max_line_width) const
    {
        std::string s = "";

        if (!m_name.empty())
        {
            s += m_name + "\n";
        }

        for (size_t i = 0; i < m_options.size(); ++i)
        {
            auto& opt = m_options[i];

            std::string flags = "  " + utils::join(", ", std::vector<std::string>(opt.flags.begin(), opt.flags.end()));

            for (const auto& flag : opt.parameters)
            {
                if (flag == A_REQUIRED_PARAMETER)
                {
                    flags += " ARG";
                }
                else
                {
                    flags += " arg";
                }
            }

            s += flags;

            for (size_t j = 0; j < fill_length - flags.size(); ++j)
            {
                s += " ";
            }

            std::string description     = opt.description;
            bool first_description_line = true;

            while (!description.empty())
            {
                if (!first_description_line)
                {
                    for (size_t j = 0; j < fill_length; ++j)
                    {
                        s += " ";
                    }
                }
                if (description.size() > max_line_width - fill_length)
                {
                    auto index = description.substr(0, max_line_width - fill_length).rfind(' ');
                    if (index == std::string::npos)
                    {
                        index = max_line_width - fill_length;
                    }
                    s += description.substr(0, index) + "\n";
                    description = description.substr(index);
                }
                else
                {
                    s += description;
                    description = "";
                }
                first_description_line = false;
            }

            if (i != m_options.size() - 1)
            {
                s += "\n";
            }
        }

        if (!s.empty())
        {
            s += "\n";
        }

        for (const auto& it : m_suboptions)
        {
            if (!it.first.empty())
            {
                s += it.first + "\n";
            }
            for (const auto& opt : it.second)
            {
                s += opt.get_options_string_internal(fill_length, max_line_width);
            }
        }

        if (s.length() > 1)
        {
            while (s.substr(s.length() - 2) != "\n\n")
            {
                s += "\n";
            }
        }
        return s;
    }
}    // namespace hal
