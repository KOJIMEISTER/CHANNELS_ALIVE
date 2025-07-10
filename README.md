# CHANNELS_ALIVE

**CHANNELS_ALIVE** is a mobile application designed to help Twitch enthusiasts track the start of their favorite Twitch channel broadcasts. Built with a combination of C++, QML, and the Qt framework, this application leverages the Twitch API to provide real-time updates and notifications on Twitch streams, ensuring you never miss when your favorite streamers go live.

![Add Channel](https://github.com/KOJIMEISTER/CHANNELS_ALIVE/blob/main/imgs/add.png)
![Main Page](https://github.com/KOJIMEISTER/CHANNELS_ALIVE/blob/main/imgs/main_page.png)
![Menu](https://github.com/KOJIMEISTER/CHANNELS_ALIVE/blob/main/imgs/menu.png)
![Notifications](https://github.com/KOJIMEISTER/CHANNELS_ALIVE/blob/main/imgs/notify.png)

## Table of Contents

- [Features](#features)
- [Technologies](#technologies)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Features

- **Add Channels:** Easily add Twitch channels by their ID to track their live status.
- **Real-Time Updates:** Receive real-time updates on subscriber counts, viewer numbers, and live status.
- **Sorting Options:** Sort your channels by the number of subscribers or by name for easy navigation.
- **Notifications:** Get notified immediately when a channel you've added goes live.
- **Intuitive UI:** A clean and user-friendly interface built with QML and Qt Quick Controls.
- **Secure Data Handling:** Utilizes AES-256 encryption to securely store and manage your data.

## Technologies

- **Programming Languages:** C++, QML
- **Framework:** Qt 6
- **APIs:** Twitch API
- **Platforms:** Android
- **Security:** AES-256 Encryption

## Installation

To build and run **CHANNELS_ALIVE** on your local machine, follow these steps:

### Prerequisites

- **Qt 6:** Ensure you have Qt 6 installed on your system. You can download it from the [official Qt website](https://www.qt.io/download).
- **C++ Compiler:** A compatible C++ compiler for your platform.
- **Android SDK & NDK:** Required for building the application for Android. Install them via the [Android Studio](https://developer.android.com/studio).

### Steps

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/KOJIMEISTER/CHANNELS_ALIVE.git
   ```

2. **Navigate to the Project Directory:**

   ```bash
   cd CHANNELS_ALIVE
   ```

3. **Open the Project:**

   Open the `CHANNELSALIVE.pro` file using Qt Creator or your preferred Qt IDE.

4. **Configure the Project:**

   - Ensure that the paths in `CHANNELSALIVE.pro` are correctly set, especially the `INCLUDEPATH` and resource paths.
   - Replace placeholder strings like `СЮДА КЛИЕНТ ID ПРИЛОЖЕНИЯ` and `СЮДА ТОКЕН КЛИЕНТ СЕКРЕТА` in `twitchapi.h` with your actual Twitch API credentials.

5. **Build the Project:**

   Select the appropriate build configuration (e.g., Debug or Release) and build the project through your IDE.

6. **Deploy to Android Device:**

   Connect your Android device or start an emulator and deploy the application using Qt Creator's deployment tools.

## Usage

1. **Launch the Application:**

   Open **CHANNELS_ALIVE** on your Android device.

2. **Add a Channel:**

   - Navigate to the "Add Channel" page.
   - Enter the Twitch channel ID and tap on "Add".
   - If the channel is found, it will be added to your tracking list.

3. **View Channels:**

   - The main page displays all your added channels.
   - Channels can be sorted by the number of subscribers or by name using the sorting dropdown.

4. **Receive Notifications:**

   - Enable notifications in the settings to receive alerts when a tracked channel goes live.

5. **Manage Channels:**

   - Access the menu to modify notification settings or remove channels from your list.

## Contributing

Contributions are welcome! If you'd like to contribute to **CHANNELS_ALIVE**, please follow these guidelines:

1. **Fork the Repository:**

   Click the "Fork" button at the top right of the repository page.

2. **Create a New Branch:**

   ```bash
   git checkout -b feature/YourFeatureName
   ```

3. **Make Your Changes:**

   Implement your feature or bug fix.

4. **Commit Your Changes:**

   ```bash
   git commit -m "Add some feature"
   ```

5. **Push to the Branch:**

   ```bash
   git push origin feature/YourFeatureName
   ```

6. **Create a Pull Request:**

   Navigate to your forked repository on GitHub and click the "New Pull Request" button.

## License

This project is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html). See the [LICENSE](https://github.com/KOJIMEISTER/CHANNELS_ALIVE/blob/main/LICENSE) file for details.

## Acknowledgments

- [Qt Framework](https://www.qt.io/) for providing the tools to build a cross-platform application.
- [Twitch API](https://dev.twitch.tv/docs/api/) for enabling integration with Twitch services.
- Special thanks to all the contributors and the open-source community for their support.
