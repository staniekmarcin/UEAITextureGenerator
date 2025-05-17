# UE AI Texture Generator Plugin (Unreal 5.3+)

Generate textures using AI directly within your UE project.

## Installation

1. Download the plugin from this repository
2. Insert the plugin into your project's "Plugins" folder (If there is no "Plugins" folder, create one)
3. Restart your project if it's currently open

## Configuration

1. Go to Project Settings -> Plugins
2. Find the AI Texture Generator plugin
3. Insert your API Key into the "OpenAI API Key" field

## Usage

1. Access the generator under Tools -> Generate AI Texture
2. Insert a prompt into the "Prompt" field
3. Click "Generate" to create a texture
4. Generated textures will be saved in Content/AI_Textures

## Enjoy Generation! 😊

If you encounter any issues or have suggestions for improvements, please open an issue in this repository.

## Why
I chose OpenAI because it provided a smooth experience with obtaining and handling requests for the initial version.

## UE Slate
Sorry for the not very modular Slate code (first time... next will be better ;) )

## TODO:
- Check if api key is valid and if not ask user to insert it
- Show created texture in plugin window
- Create checkbox if original PNG should be deleted
- Add more models
- Add style switcher
- Handle more edge cases
- Give users choice of texture parameters
- Give users ability to change the folder where textures will be saved
- Add images to README for better readability
- Add switcher for how many versions of images should be generated