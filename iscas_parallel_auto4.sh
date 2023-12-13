#!/bin/bash

# Specify the folder containing .v files
folder_path="./netlists/ISCAS85-netlists"

# Log file to record success/failure status
log_file="parallel_execution_log.txt"


# Loop through each .v file in the folder
for file_path in "$folder_path"/*.v; do

    

    # Check if there are .v files in the folder
    if [ -e "$file_path" ]; then

        base_name=$(basename "$file_path")
        output_file="${base_name}_output.txt"

        # Run the Python script in parallel for each .v file
        echo "Running depth4 for " "$file_path"
        python3 HT_detection.py 4 "$file_path" > "$output_file" 2>&1 &

        job_number=$!
        echo "Job number: $job_number"
        
	echo "Running depth inf for " "$file_path"
        python3 HT_detection.py -1 "$file_path" > "$output_file" 2>&1 &

        job_number=$!
        echo "Job number: $job_number"

    fi
done

# Wait for all background processes to finish
wait
