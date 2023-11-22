#include "dataflow_analysis/processing/passes/merge_successor_predecessor_groupings.h"

#include "dataflow_analysis/common/netlist_abstraction.h"
#include "dataflow_analysis/processing/configuration.h"
#include "dataflow_analysis/processing/passes/group_by_successors_predecessors.h"
#include "dataflow_analysis/processing/passes/merge_states.h"

namespace hal
{
    namespace dataflow
    {
        namespace merge_successor_predecessor_groupings
        {
            std::shared_ptr<Grouping> process(const processing::Configuration& config, const std::shared_ptr<Grouping>& state, bool delete_from_smaller)
            {
                auto new_state1 = group_by_successors_predecessors::process(config, state, true);
                auto new_state2 = group_by_successors_predecessors::process(config, state, false);

                auto merged_state = merge_states::process(config, new_state1, new_state2, delete_from_smaller);

                return merged_state;
            }

        }    // namespace merge_successor_predecessor_groupings
    }        // namespace dataflow
}    // namespace hal