# Native Wayland System Monitor

![System Dashboard](screenshot.jpg)

A lightweight, Python-based (Qt) native system dashboard optimized for Hyprland. 

This native Wayland application provides a sleek, Bento Box-style masonry layout that monitors your core system metrics, background services, and system alerts without the overhead of electron or web-based dashboards.

## Features

- **Zero-overhead background polling:** Aggressively caches system updates with a 60-minute package cache.
- **Passive systemd timer monitoring:** Tracks the active state of background services like the Filen CLI syncing engine without actively scanning your disk.
- **Live thermal tracking and active socket monitoring:** Keeps a close eye on your CPU/hardware package temperatures (with auto-suspend safeties built-in) and actively open network sockets.
- **Filtered journalctl and systemctl alert tracking:** Features dynamic Chip toggle buttons to instantly parse critical hardware events, full kernel activity, and broken/crashing systemd services instantly.

## Installation

1. Clone or download the repository into your preferred folder:
   ```bash
   git clone https://github.com/bgonc/bgonc.git
   cd bgonc/system_dashboard
   ```

2. Create a Python virtual environment and activate it:
   ```bash
   python -m venv venv
   source venv/bin/activate
   ```

3. Install the required dependencies:
   ```bash
   pip install -r requirements.txt
   ```

## Usage

You can launch the dashboard directly using the provided shell script:

```bash
./run.sh
```

**Note:** The application uses `QT_QPA_PLATFORM=wayland` inside the shell script to force crisp native Wayland rendering on Hyprland.

<img width="1920" height="1050" alt="image" src="https://github.com/user-attachments/assets/60aac144-2dbf-4a49-a73d-7e8ac9c22453" />

