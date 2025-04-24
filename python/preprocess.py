import subprocess
import pandas as pd

columns = ['baseline_nonsharing','nyx_nonsharing','baseline_fcfs','nyx_fcfs','baseline_rr','nyx_rr','baseline_sjf','nyx_sjf','baseline_nimblock','nyx_nimblock']

for column in columns:
    system, scheduler = column.split('_')
    command = ["bash", "reorder.sh", "baseline_nonsharing.csv", f"{system}_{scheduler}.csv"]
    if column=='baseline_nonsharing':
        continue
    print(command)
    with open(f"{system}_{scheduler}_reordered.csv", "w") as f:
        result=subprocess.run(command, capture_output=True, text=True, check=True)
        f.write(result.stdout)
subprocess.run("cp baseline_nonsharing.csv baseline_nonsharing_reordered.csv", shell=True, executable="/bin/bash")

# Initialize the merged DataFrame with the first file's first column

merged_df = pd.DataFrame()
for i, file in enumerate(columns):
    df = pd.read_csv(file+"_reordered.csv", header=None) 
    if i == 0:
        # For the first file, we store the first column
        merged_df["App_Batch"] = df[0].astype(str) + '_' + df[1].astype(str)
        merged_df[file] = df.iloc[:, 2]
        print(merged_df['App_Batch'])
        
    else:
        if i%2 == 0:
            merged_df[file] = df.iloc[:, 2]  # Add second column from this file
        else:
            merged_df[file] = df.iloc[:, 1]  # Add second column from this file


# Save the merged DataFrame to a new Excel file
print(merged_df)
merged_df.to_csv('results.csv', index=False)
