import subprocess

def run_script(script_name):
    try:
        subprocess.run(["python3", script_name])
    except Exception as e:
        print(f"Error running script: {e}")

if __name__ == "__main__":
    print("Select a script to run:")
    print("1. process_data.py")
    print("2. linear_regression.py")
    print("3. analyze.py")
    print("4. MQTTdata_mongo.py")

    choice = input("Enter the number of the script to run (1-4): ")

    if choice == "1":
        run_script("process_data.py")
    elif choice == "2":
        run_script("linear_regression.py")
    elif choice == "3":
        run_script("analyze.py")
    elif choice == "4":
        run_script("MQTTdata_mongo.py")
    else:
        print("Invalid choice. Please enter a number between 1 and 4.")
