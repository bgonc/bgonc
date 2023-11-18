import openai
import os

# Define the message_prompt function before you use it
def message_prompt(messages):
    """Formats the conversation for submitting to the OpenAI API."""
    return "".join(
        f"{msg['role'].capitalize()}: {msg['content']}\n" for msg in messages
    )

# Load your API key from an environment variable or secret management service
api_key = os.getenv('OPENAI_API_KEY')
if api_key is None:
    raise ValueError("Please set the OPENAI_API_KEY environment variable.")
openai.api_key = api_key

# Define the initial system message
system_message = {
    "role": "system",
    "content": "You are a helpful assistant."
}

# Start a conversation with an empty history
conversation_history = [system_message]

# Start an interactive loop
while True:
    try:
        # Take input from the user
        user_input = input("You: ")

        # Check if the user wants to exit the conversation
        if user_input.lower() == 'exit':
            print("Exiting the conversation.")
            break

        # Append the user's message to the conversation history
        conversation_history.append({"role": "user", "content": user_input})

        # Call the OpenAI API to generate a response
        response = openai.ChatCompletion.create(
            model="gpt-4-1106-preview",
            messages=conversation_history
        )

        # Extract the text from the response
        message_content = response['choices'][0]['message']['content'].strip()

        # Print the AI's response
        print("AI:", message_content)

        # Append the AI's response to the conversation history
        conversation_history.append({"role": "assistant", "content": message_content})

    except openai.OpenAIError as e:
        print(f"An error occurred: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
