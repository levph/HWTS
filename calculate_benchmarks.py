import os
import pandas as pd

Trojan_Mapping = {
    # RS323 Family
    "RS232-T1100-uart.v": ("U293", "U294", "U295", "U296", "U297", "U298", "U299", "U300", "U301", "U302", "U305"),
    "RS232-T1200-uart.v": ("U292", "U293", "U294", "U295", "U296", "U297", "U300", "U301", "U302", "U303"),
    "RS232-T1300-uart.v": ("U292", "U293", "U294", "U295", "U296", "U297", "U302", "U303", "U304"),
    "RS232-T1400-uart.v": ("U292", "U293", "U294", "U295", "U296", "U297", "U298", "U299", "U300", "U301", "U302", "U303"),
    "RS232-T1500-uart.v": ("U293", "U294", "U295", "U296", "U297", "U298", "U299", "U300", "U301", "U302", "U303", "U304", "U305"),
    "RS232-T1600-uart.v": ("U293", "U294", "U295", "U296", "U297", "U300", "U301", "U302", "U303", "U304"),

    # s15850 Family todo: change file name after finish
    "s15850-T100_scan_free.v": (),
    "s15850_scan_in.v": ("Tg1_Trojan1", "Tg1_Trojan2", "Tg1_Trojan3", "Tg1_Trojan4", "Tg1_Trojan1234",
                         "Tg1_Trojan5", "Tg1_Trojan6", "Tg1_Trojan7", "Tg1_Trojan8", "Tg1_Trojan5678", "Tg1_Tj_Trigger",
                         "Tg2_Trojan1", "Tg2_Trojan2", "Tg2_Trojan3", "Tg2_Trojan4", "Tg2_Trojan1234",
                         "Tg2_Trojan5", "Tg2_Trojan6", "Tg2_Trojan7", "Tg2_Trojan8", "Tg2_Trojan5678", "Tg2_Tj_Trigger",
                         "INVtest_se", "Trojan_Trigger", "Trojan_Payload"),

    # s35932 Family
    "s35932-T100_scan_free.v": (),
    "s35932-T100_scan_in.v": ("Trojan1", "Trojan2", "Trojan3", "Trojan4", "Trojan1234_NOT", "Trojan5", "Trojan6", "Trojan7", "Trojan8", "Trojan5678_NOT",
                              "INV_test_se", "Trojan_Trigger", "Trojan_Payload1", "Trojan_Payload2"),
    "s35932-T200_scan_free.v": (),
    "s35932-T200_scan_in.v": ("Trojan1", "Trojan2", "Trojan3", "Trojan4", "Trojan1234_NOT", "Trojan5", "Trojan6", "Trojan7", "Trojan8", "Trojan5678_NOT",
                              "INVtest_se", "Trojan_Trigger", "U5548", "U5566", "U6740", "U6802"),
    "s35932-T300_scan_free.v": (),
    "s35932-T300_scan_in.v": ("Trojan1", "Trojan2", "Trojan3", "Trojan4", "Trojan1234_NOT", "Trojan5", "Trojan6", "Trojan7", "Trojan8", "Trojan5678_NOT",
                              "INVtest_se", "Trojan_Trigger", "TjPayload1", "TjPayload2", "TjPayload4", "TjPayload6", "TjPayload7", "TjPayload9",
                              "TjPayload11", "TjPayload12", "TjPayload13", "TjPayload15", "TjPayload17", "TjPayload18", "TjPayload19", "TjPayload21", "TjPayload23"),

    # s38417 Family todo: change file name after finish
    "s38417-T100_scan_free.v": (),
    "s38417_scan_in.v": ("Trojan1", "Trojan2", "Trojan3", "Trojan4", "Trojan1234_NOT", "Trojan5", "Trojan6", "Trojan7", "Trojan8", "Trojan5678_NOT",
                         "Trojan_CLK_NOT", "Trojan_Payload")
}

NOPE = ["s38417_scan_in.v", "s15850_scan_in.v"]

NETLISTS_FOLDER = "netlists/"

def list_files_in_folder(folder_path):
    # Get a list of all files and subdirectories in the given folder
    files = os.listdir(folder_path)

    # Filter out subdirectories, leaving only files
    files = sorted([f[0:len(f)-2] for f in files if os.path.isfile(os.path.join(folder_path, f)) and f in Trojan_Mapping.keys()]) # and not f in NOPE

    return files

print(list_files_in_folder(NETLISTS_FOLDER))
threshold = 0.025

for netlist in list_files_in_folder(NETLISTS_FOLDER):
    files = os.listdir(os.path.join("output/", netlist))
    files = sorted([f for f in files if os.path.isfile(os.path.join(os.path.join("output/", netlist), f)) and ("lim4" in f or "lim2" in f) and not f.startswith('.')])
    print(files)
    if len(files):
        inf_file = files[1]
        if len(files) == 2:
            inf_file = ""
            lim2_file = ""
            if "inf" in files[0]:
                inf_file = files[0]
                lim2_file = files[1]
            else:
                inf_file = files[1]
                lim2_file = files[0]


        df = pd.read_csv(os.path.join("output/", netlist, inf_file))
        sorted_df = df.sort_values(by='MEAN')
        netlist_file = netlist + ".v"

        filtered_df = df[df['MEAN'] <= threshold]
        print(filtered_df)

        # List of pre-defined names to filter out
        gates_to_exclude = list(Trojan_Mapping[netlist_file])

        # Filter out rows based on the pre-defined list of names
        filtered_df_excluded = filtered_df[~filtered_df['GATE_NAME'].isin(gates_to_exclude)]
        print(filtered_df_excluded)

        proportion = len(filtered_df_excluded) / float(len(df))

        print("Proportion of lines with 'MEAN' <" + str(threshold) + ":", proportion*100)

        if len(files) == 2:

            inf_df = pd.read_csv(os.path.join("output/", netlist, inf_file))
            lim2_df = pd.read_csv(os.path.join("output/", netlist, lim2_file))

            diff_column = abs(inf_df['MEAN'] - lim2_df['MEAN']) / inf_df['MEAN']

            new_df = pd.DataFrame({'GATE_NAME': inf_df['GATE_NAME'], 'GATE_TYPE': inf_df['GATE_TYPE'],
                                   'MEAN_DIFF': diff_column}).sort_values(by='MEAN_DIFF', ascending=False)

            filtered_df = new_df[new_df['MEAN_DIFF'] >= 1]
            print(filtered_df)

            # List of pre-defined names to filter out
            gates_to_exclude = list(Trojan_Mapping[netlist_file])

            # Filter out rows based on the pre-defined list of names
            filtered_df_excluded = filtered_df[~filtered_df['GATE_NAME'].isin(gates_to_exclude)]
            print(filtered_df_excluded)

            proportion = len(filtered_df_excluded) / float(len(inf_df))

            print("Proportion of lines with 'MEAN_DIFF' > 1:", proportion * 100)

