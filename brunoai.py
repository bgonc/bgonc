import openai

# Set your API key
openai.api_key = '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'

# Define the system message
system_message = {
    "role": "system",
    "content": "You are a helpful assistant."
}

# Define the conversation list with the system message included
conversation = [system_message]

# Function to create a chat message
def create_chat(user_input):
    conversation.append({"role": "user", "content": user_input})
    response = openai.ChatCompletion.create(
        model="gpt-4-1106-preview",  # Make sure to use the correct model
        messages=conversation
    )
    assistant_message = response['choices'][0]['message']['content']
    conversation.append({"role": "assistant", "content": assistant_message})
    return assistant_message

# Example usage
user_input = "What is the weather like today?"
assistant_response = create_chat(user_input)
print("AI:", assistant_response)
