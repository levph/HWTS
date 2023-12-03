import os
import subprocess


def run_script(file_path, param1):
    # Construct the command to run the script with parameters
    script_path = "HT_detection.py"
    command = ["python", script_path, param1, file_path]

    # Run the command
    try:
        print("\n\nRUNNING FOR " + param1 + "\n\n")
        subprocess.run(command, check=True)
        print(f"Script executed successfully for file: {file_path}")
    except subprocess.CalledProcessError as e:
        print(f"Error executing script for file {file_path}: {e}")


def process_files(folder_path, param1, param2):
    # Get a list of files in the folder
    files = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]

    # Run the script for each file with different parameters
    for file in files:
        file_path = os.path.join(folder_path, file)

        # Run the script with the first set of parameters
        run_script(file_path, param1)

        # Run the script with the second set of parameters
        run_script(file_path, param2)


if __name__ == "__main__":
    # Specify the folder and parameters
    folder_path = "netlists/verilog-itc-netlists"

    param1 = "2"
    param2 = "-1"

    # Call the function to process files
    process_files(folder_path, param1, param2)

