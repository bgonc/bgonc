# Ask the user for the filename
filename = input("Enter the filename you want to create or append to (with extension, e.g., 'data.txt'): ")

# Ask for the user's email
email = input("Your email: ")

# Ask the user if they want to create a new file or append to the existing one
action = input("Type 'create' to create a new file, or 'append' to add to an existing file: ")

if action.lower() == 'create':
        # Open the file in 'w' mode to create a new file or overwrite an existing one
        file_mode = "w"
elif action.lower() == 'append':
        # Open the file in 'a' mode to append to an existing file or create a new one if it doesn't exist
        file_mode = "a"
else:
        print("Invalid action. Only 'create' or 'append' are accepted.")
        exit()

    # Open the file with the chosen mode
with open(filename, file_mode) as file:
        file.write(email + "\n")  # Write the email and add a newline
    
    # Read and print the file content if it was created or appended successfully
with open(filename, "r") as file:
        print("File content:")
        content = file.read()
        print(content)