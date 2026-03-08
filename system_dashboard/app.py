import sys
import os
import subprocess
import re
import json
import time

try:
    import psutil
except ImportError:
    psutil = None

from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QGridLayout,
    QLabel, QScrollArea, QFrame, QSizePolicy, QProgressBar, QTabWidget, QPushButton
)
from PyQt6.QtCore import Qt, QTimer, QThread, pyqtSignal

ENV = os.environ.copy()
if "/home/bruno/.local/bin" not in ENV.get("PATH", ""):
    ENV["PATH"] = f"/home/bruno/.local/bin:{ENV.get('PATH', '')}"

def run_cmd(cmd, timeout=8):
    try:
        result = subprocess.run(
            cmd,
            shell=True,
            env=ENV,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        return result.stdout.strip()
    except subprocess.TimeoutExpired:
        return f"Timeout executing: {cmd}"
    except Exception as e:
        return f"Error: {str(e)}"

class WorkerThread(QThread):
    result_ready = pyqtSignal(str, str)

    def __init__(self, identifier, cmd, timeout=8):
        super().__init__()
        self.identifier = identifier
        self.cmd = cmd
        self.timeout = timeout

    def run(self):
        output = run_cmd(self.cmd, self.timeout)
        self.result_ready.emit(self.identifier, output)

class ClickableCard(QFrame):
    clicked = pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setProperty("class", "InteractiveCard")

    def mousePressEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            self.clicked.emit()
        super().mousePressEvent(event)

class DashboardWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.kernel_cmd = "journalctl -k -p 3 -n 5 --no-pager"
        
        self.setWindowTitle("System Dashboard")
        self.setWindowFlags(Qt.WindowType.FramelessWindowHint | Qt.WindowType.WindowStaysOnTopHint)
        self.setAttribute(Qt.WidgetAttribute.WA_TranslucentBackground)

        self.thermal_critical = False
        self.systemd_mode = "broken"
        self.last_net_io = psutil.net_io_counters() if psutil else None
        self.threads = []

        self.setup_ui()
        self.setup_timers()
        
        self.check_updates()
        self.check_services()
        self.check_thermal()

    def create_card(self, title, is_expanding=False, interactive=False):
        card = ClickableCard() if interactive else QFrame()
        if not interactive:
            card.setProperty("class", "Card")
            
        if is_expanding:
            card.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        else:
            card.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
            
        layout = QVBoxLayout(card)
        layout.setContentsMargins(0, 0, 0, 0)
        
        if title:
            header = QLabel(title)
            header.setProperty("class", "CardHeader")
            header.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents)
            layout.addWidget(header)
            
        return card, layout

    def create_data_label(self, text, color=None, fontsize=24, is_sub=False):
        lbl = QLabel(text)
        lbl.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents)
        if is_sub:
            lbl.setProperty("class", "SubText")
        else:
            lbl.setProperty("class", "DataText")
            style = f"font-size: {fontsize}px;"
            if color: style += f" color: {color};"
            lbl.setStyleSheet(style)
        return lbl

    def setup_ui(self):
        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        
        self.normal_style = """
            #MainWindow {
                background-color: rgba(2, 6, 23, 0.85);
                border: 1px solid rgba(255, 255, 255, 0.08);
                border-radius: 16px;
            }
            QLabel { color: #f8fafc; font-family: 'Inter', sans-serif; }
            .Card {
                background-color: rgba(255, 255, 255, 0.03);
                border: 1px solid rgba(255, 255, 255, 0.08);
                border-radius: 16px;
            }
            .InteractiveCard {
                background-color: rgba(255, 255, 255, 0.05);
                border: 1px solid rgba(255, 255, 255, 0.08);
                border-radius: 16px;
            }
            .InteractiveCard:hover {
                background-color: rgba(255, 255, 255, 0.1);
                border: 1px solid rgba(255, 255, 255, 0.2);
            }
            .CardHeader {
                font-weight: bold;
                font-size: 16px;
                padding: 12px 12px 4px 12px;
            }
            .DataText {
                font-family: 'Fira Code', monospace;
                font-weight: bold;
                padding: 0px 12px;
            }
            .SubText {
                font-size: 14px;
                color: #94a3b8;
                padding: 4px 12px 12px 12px;
            }
            .AlertText, .ProcText, .TabMonospace {
                font-family: 'Fira Code', monospace;
                font-size: 10pt;
                padding: 12px;
            }
            .AlertText { color: #fca5a5; }
            .ProcText { color: #f8fafc; }
            .HeaderStats {
                font-family: 'Fira Code', monospace;
                font-size: 14px;
                font-weight: bold;
            }
            .ChipButton {
                background: transparent;
                border: 1px solid rgba(255, 255, 255, 0.1);
                border-radius: 6px;
                padding: 4px 12px;
                color: #94a3b8;
                font-family: 'Inter', sans-serif;
                font-size: 11px;
                font-weight: 600;
            }
            .ChipButton:hover {
                background: rgba(255, 255, 255, 0.05);
                color: #f8fafc;
            }
            .ChipActive {
                background: rgba(59, 130, 246, 0.3);
                border: 1px solid rgba(59, 130, 246, 0.5);
                border-radius: 6px;
                padding: 4px 12px;
                color: #f8fafc;
                font-family: 'Inter', sans-serif;
                font-size: 11px;
                font-weight: 600;
            }
            
            QProgressBar {
                background-color: rgba(0, 0, 0, 0.3);
                border-radius: 3px;
                border: none;
            }
            QProgressBar::chunk {
                background-color: #3b82f6;
                border-radius: 3px;
            }
            
            QTabWidget::pane {
                border: 1px solid rgba(255, 255, 255, 0.08);
                border-radius: 12px;
                background-color: rgba(255, 255, 255, 0.02);
            }
            QTabBar::tab {
                background: rgba(255, 255, 255, 0.05);
                border: 1px solid rgba(255, 255, 255, 0.08);
                border-radius: 8px;
                padding: 6px 16px;
                margin-right: 4px;
                margin-bottom: 8px;
                color: #94a3b8;
                font-family: 'Inter', sans-serif;
                font-weight: 600;
            }
            QTabBar::tab:selected {
                background: rgba(59, 130, 246, 0.2);
                color: #f8fafc;
                border: 1px solid rgba(59, 130, 246, 0.5);
            }
            
            QScrollArea, QScrollArea > QWidget > QWidget {
                border: none;
                background-color: transparent;
            }
            QScrollBar:vertical {
                border: none;
                background: rgba(255, 255, 255, 0.05);
                width: 6px;
                margin: 4px 0 4px 0;
                border-radius: 3px;
            }
            QScrollBar::handle:vertical {
                background: rgba(255, 255, 255, 0.2);
                min-height: 20px;
                border-radius: 3px;
            }
            QScrollBar::handle:vertical:hover {
                background: rgba(255, 255, 255, 0.4);
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                border: none;
                background: none;
            }
            QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                background: none;
            }

            QComboBox {
                background: rgba(255, 255, 255, 0.05);
                border: 0px;
                outline: none;
                border-radius: 6px;
                padding: 4px 8px;
                color: #f8fafc;
                font-family: 'Inter', sans-serif;
                font-size: 11px;
            }
            QComboBox::drop-down {
                border: none;
            }
            QComboBox QAbstractItemView {
                background-color: #0f172a;
                color: #f8fafc;
                selection-background-color: rgba(59, 130, 246, 0.5);
                border: 0px;
                outline: none;
                border-radius: 6px;
            }
            QComboBox QAbstractItemView {
                background-color: #0f172a;
                color: #f8fafc;
                selection-background-color: rgba(59, 130, 246, 0.5);
                border: 1px solid rgba(255, 255, 255, 0.1);
                border-radius: 6px;
            }
        """
        self.critical_style = self.normal_style.replace("border: 1px solid rgba(255, 255, 255, 0.08);", "border: 2px solid #ef4444;")
        
        self.central_widget.setObjectName("MainWindow")
        self.central_widget.setStyleSheet(self.normal_style)
        
        main_layout = QVBoxLayout(self.central_widget)
        main_layout.setContentsMargins(24, 24, 24, 24)
        main_layout.setSpacing(16)

        # ================= HEADER CONTAINER =================
        header_container = QFrame()
        header_container.setProperty("class", "Card")
        header_container.setStyleSheet("background-color: rgba(2, 6, 23, 0.95); border-radius: 12px; padding: 8px;")
        h_layout = QHBoxLayout(header_container)
        h_layout.setContentsMargins(16, 8, 16, 8)
        
        self.lbl_hdr_cpu = QLabel("CPU: 0%")
        self.lbl_hdr_cpu.setProperty("class", "HeaderStats")
        self.lbl_hdr_ram = QLabel("RAM: 0/0GB")
        self.lbl_hdr_ram.setProperty("class", "HeaderStats")
        self.lbl_hdr_net = QLabel("Net: ↓0 ↑0 KB/s")
        self.lbl_hdr_net.setProperty("class", "HeaderStats")
        self.lbl_bat_pct = QLabel("BAT: AC")
        self.lbl_bat_pct.setProperty("class", "HeaderStats")
        self.lbl_bat_pct.setStyleSheet("color: #10b981;")
        self.lbl_hw_temp = QLabel("TMP: N/A")
        self.lbl_hw_temp.setProperty("class", "HeaderStats")
        self.lbl_hw_temp.setStyleSheet("color: #ef4444;")
        
        h_layout.addWidget(self.lbl_hdr_cpu)
        h_layout.addStretch()
        h_layout.addWidget(self.lbl_hdr_ram)
        h_layout.addStretch()
        h_layout.addWidget(self.lbl_hdr_net)
        h_layout.addStretch()
        h_layout.addWidget(self.lbl_bat_pct)
        h_layout.addStretch()
        h_layout.addWidget(self.lbl_hw_temp)
        
        main_layout.addWidget(header_container)

        # ================= MASONRY BENTO GRID =================
        grid_layout = QHBoxLayout()
        grid_layout.setSpacing(16)

        # ---------------- COLUMN 1: System Updates + Net Sockets ----------------
        col1 = QVBoxLayout()
        col1.setSpacing(16)
        col1.setAlignment(Qt.AlignmentFlag.AlignTop)
        
        updates_card, upd_layout = self.create_card("System Updates\n(Click to Update)", interactive=True)
        updates_card.clicked.connect(self.launch_updates)
        self.lbl_pacman = self.create_data_label("...", color="#f59e0b")
        upd_layout.addWidget(self.lbl_pacman)
        upd_layout.addWidget(self.create_data_label("Pacman Packages", is_sub=True))
        
        self.lbl_aur = self.create_data_label("...", color="#3b82f6")
        upd_layout.addWidget(self.lbl_aur)
        upd_layout.addWidget(self.create_data_label("AUR (yay) Packages", is_sub=True))
        col1.addWidget(updates_card)
        
        net_card, net_layout = self.create_card("Active Net Sockets", is_expanding=True)
        self.lbl_net_sockets = QLabel("Fetching...")
        self.lbl_net_sockets.setProperty("class", "ProcText")
        self.lbl_net_sockets.setWordWrap(True)
        self.lbl_net_sockets.setAlignment(Qt.AlignmentFlag.AlignTop)
        
        ns_scroll = QScrollArea()
        ns_scroll.setWidgetResizable(True)
        ns_scroll.setFrameShape(QFrame.Shape.NoFrame)
        ns_scroll.setStyleSheet("background: transparent;")
        ns_scroll.setWidget(self.lbl_net_sockets)
        net_layout.addWidget(ns_scroll)
        col1.addWidget(net_card)

        # ---------------- COLUMN 2: Core Services + Filen Sync ----------------
        col2 = QVBoxLayout()
        col2.setSpacing(16)
        col2.setAlignment(Qt.AlignmentFlag.AlignTop)
        
        services_card, srv_layout = self.create_card("Core Services")
        self.lbl_filen = self.create_data_label("Loading...", color="#10b981", fontsize=16)
        srv_layout.addWidget(self.lbl_filen)
        srv_layout.addWidget(self.create_data_label("Filen Timer (Next Sync)", is_sub=True))
        
        self.lbl_workspaces = self.create_data_label("Loading...", fontsize=12)
        self.lbl_workspaces.setStyleSheet("font-size: 12px; font-family: 'Inter', sans-serif; padding: 0 12px;")
        self.lbl_workspaces.setWordWrap(True)
        srv_layout.addWidget(self.lbl_workspaces)
        srv_layout.addWidget(self.create_data_label("Active Workspaces", is_sub=True))
        
        self.lbl_sys_services = self.create_data_label("dbus: ? | Net: ? | blue: ?", fontsize=12)
        self.lbl_sys_services.setStyleSheet("font-size: 12px; font-family: 'Inter', sans-serif; padding: 0 12px 12px 12px; color: #94a3b8;")
        srv_layout.addWidget(self.lbl_sys_services)
        
        # Disk Bar (Thin)
        disk_container = QWidget()
        disk_layout = QVBoxLayout(disk_container)
        disk_layout.setContentsMargins(12, 0, 12, 12)
        self.lbl_disk_title = QLabel("Root (/) - ? Free")
        self.lbl_disk_title.setStyleSheet("font-size: 12px; color: #f8fafc; font-family: 'Inter', sans-serif;")
        
        bar_layout = QHBoxLayout()
        self.disk_bar = QProgressBar()
        self.disk_bar.setMinimum(0)
        self.disk_bar.setMaximum(100)
        self.disk_bar.setValue(0)
        self.disk_bar.setFixedHeight(6)
        self.disk_bar.setTextVisible(False)
        
        self.lbl_disk_pct = QLabel("0%")
        self.lbl_disk_pct.setStyleSheet("font-family: 'Fira Code', monospace; font-size: 10pt; color: #f8fafc; font-weight: bold;")
        
        bar_layout.addWidget(self.disk_bar)
        bar_layout.addWidget(self.lbl_disk_pct)
        
        disk_layout.addWidget(self.lbl_disk_title)
        disk_layout.addLayout(bar_layout)
        srv_layout.addWidget(disk_container)
        col1.addWidget(services_card)
        col1.addWidget(net_card)
        
        # Condensed Filen Sync Manager
        filen_card, filen_layout = self.create_card(None, is_expanding=True)
        filen_layout.setContentsMargins(12, 12, 12, 12)
        
        f_hdr_layout = QHBoxLayout()
        f_title = QLabel("Filen Sync Manager")
        f_title.setStyleSheet("font-weight: bold; font-size: 16px; padding-left: 4px; padding-bottom: 8px;")
        f_hdr_layout.addWidget(f_title)
        
        f_hdr_layout.addStretch()
        self.lbl_filen_status_dot = QLabel("●")
        self.lbl_filen_status_dot.setStyleSheet("color: #94a3b8; font-size: 16px;")
        f_hdr_layout.addWidget(self.lbl_filen_status_dot)
        
        filen_layout.addLayout(f_hdr_layout)
        
        self.filen_tabs = QTabWidget()
        
        # Tab 1: Folders
        self.sync_area = QScrollArea()
        self.sync_area.setWidgetResizable(True)
        self.sync_area.setFrameShape(QFrame.Shape.NoFrame)
        self.sync_area.setStyleSheet("background: transparent;")
        
        self.sync_container = QWidget()
        self.sync_grid = QGridLayout(self.sync_container)
        self.sync_grid.setContentsMargins(8, 8, 8, 8)
        self.sync_grid.setSpacing(12)
        
        self.sync_area.setWidget(self.sync_container)
        self.filen_tabs.addTab(self.sync_area, "Folders")
        
        # Tab 2: Logs
        self.filen_logs_lbl = QLabel("Fetching logs...")
        self.filen_logs_lbl.setProperty("class", "TabMonospace")
        self.filen_logs_lbl.setStyleSheet("color: #94a3b8; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 12px;")
        self.filen_logs_lbl.setWordWrap(True)
        self.filen_logs_lbl.setAlignment(Qt.AlignmentFlag.AlignTop)
        
        logs_scroll = QScrollArea()
        logs_scroll.setWidgetResizable(True)
        logs_scroll.setFrameShape(QFrame.Shape.NoFrame)
        logs_scroll.setStyleSheet("background: transparent;")
        logs_scroll.setWidget(self.filen_logs_lbl)
        
        self.filen_tabs.addTab(logs_scroll, "Recent Logs")
        
        # Tab 3: Quota
        quota_widget = QWidget()
        quota_layout = QVBoxLayout(quota_widget)
        quota_layout.setContentsMargins(12, 12, 12, 12)
        quota_layout.setAlignment(Qt.AlignmentFlag.AlignTop)
        
        self.lbl_quota_text = QLabel("Fetching quota...")
        self.lbl_quota_text.setStyleSheet("font-size: 12px; color: #f8fafc; font-family: 'Inter', sans-serif;")
        
        q_bar_layout = QHBoxLayout()
        self.quota_bar = QProgressBar()
        self.quota_bar.setMinimum(0)
        self.quota_bar.setMaximum(100)
        self.quota_bar.setValue(0)
        self.quota_bar.setFixedHeight(6)
        self.quota_bar.setTextVisible(False)
        
        self.lbl_quota_pct = QLabel("0%")
        self.lbl_quota_pct.setStyleSheet("font-family: 'Fira Code', monospace; font-size: 10pt; color: #f8fafc; font-weight: bold;")
        
        q_bar_layout.addWidget(self.quota_bar)
        q_bar_layout.addWidget(self.lbl_quota_pct)
        
        quota_layout.addWidget(self.lbl_quota_text)
        quota_layout.addLayout(q_bar_layout)
        
        self.filen_tabs.addTab(quota_widget, "Account Quota")
        
        filen_layout.addWidget(self.filen_tabs)
        
        col3 = QVBoxLayout()
        col3.setSpacing(16)
        col3.setAlignment(Qt.AlignmentFlag.AlignTop)
        col3.addWidget(filen_card)
        
        alerts_card, al_layout = self.create_card(None, is_expanding=True)
        al_layout.setContentsMargins(12, 12, 12, 12)
        
        # Alerts Title row
        alerts_hdr_layout = QHBoxLayout()
        alerts_title = QLabel("System Alerts")
        alerts_title.setStyleSheet("font-weight: bold; font-size: 16px; padding-left: 4px;")
        alerts_hdr_layout.addWidget(alerts_title)
        alerts_hdr_layout.addStretch()
        al_layout.addLayout(alerts_hdr_layout)
        
        self.alerts_tabs = QTabWidget()
        self.alerts_tabs.currentChanged.connect(self.on_alerts_tab_changed)
        
        # Alerts Tab 1: Kernel
        kernel_widget = QWidget()
        kernel_layout = QVBoxLayout(kernel_widget)
        kernel_layout.setContentsMargins(0, 0, 0, 0)
        kernel_layout.setSpacing(8)
        
        # Kernel Chips
        k_chips_layout = QHBoxLayout()
        k_chips_layout.setContentsMargins(12, 8, 12, 0)
        self.btn_k_crit = QPushButton("Critical (-p 3)")
        self.btn_k_crit.setProperty("class", "ChipActive")
        self.btn_k_crit.setCursor(Qt.CursorShape.PointingHandCursor)
        self.btn_k_crit.clicked.connect(lambda: self.set_kernel_filter("critical"))
        
        self.btn_k_full = QPushButton("Full (-n 50)")
        self.btn_k_full.setProperty("class", "ChipButton")
        self.btn_k_full.setCursor(Qt.CursorShape.PointingHandCursor)
        self.btn_k_full.clicked.connect(lambda: self.set_kernel_filter("full"))
        
        k_chips_layout.addWidget(self.btn_k_crit)
        k_chips_layout.addWidget(self.btn_k_full)
        k_chips_layout.addStretch()
        kernel_layout.addLayout(k_chips_layout)
        
        kernel_scroll = QScrollArea()
        kernel_scroll.setWidgetResizable(True)
        kernel_scroll.setFrameShape(QFrame.Shape.NoFrame)
        kernel_scroll.setStyleSheet("background: transparent;")
        
        self.lbl_kernel_alerts = QLabel("Checking kernel logs...")
        self.lbl_kernel_alerts.setProperty("class", "AlertText")
        self.lbl_kernel_alerts.setStyleSheet("color: #94a3b8; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 16px;")
        self.lbl_kernel_alerts.setWordWrap(True)
        self.lbl_kernel_alerts.setAlignment(Qt.AlignmentFlag.AlignTop)
        
        kernel_scroll.setWidget(self.lbl_kernel_alerts)
        kernel_layout.addWidget(kernel_scroll)
        self.alerts_tabs.addTab(kernel_widget, "Kernel")
        
        # Alerts Tab 2: Systemd
        systemd_widget = QWidget()
        systemd_layout = QVBoxLayout(systemd_widget)
        systemd_layout.setContentsMargins(0, 0, 0, 0)
        systemd_layout.setSpacing(8)
        
        # Systemd Chips
        s_chips_layout = QHBoxLayout()
        s_chips_layout.setContentsMargins(12, 8, 12, 0)
        self.btn_s_broken = QPushButton("Broken")
        self.btn_s_broken.setProperty("class", "ChipActive")
        self.btn_s_broken.setCursor(Qt.CursorShape.PointingHandCursor)
        self.btn_s_broken.clicked.connect(lambda: self.set_systemd_filter("broken"))
        
        self.btn_s_running = QPushButton("Running")
        self.btn_s_running.setProperty("class", "ChipButton")
        self.btn_s_running.setCursor(Qt.CursorShape.PointingHandCursor)
        self.btn_s_running.clicked.connect(lambda: self.set_systemd_filter("running"))
        
        self.btn_s_stopped = QPushButton("Stopped")
        self.btn_s_stopped.setProperty("class", "ChipButton")
        self.btn_s_stopped.setCursor(Qt.CursorShape.PointingHandCursor)
        self.btn_s_stopped.clicked.connect(lambda: self.set_systemd_filter("stopped"))
        
        s_chips_layout.addWidget(self.btn_s_broken)
        s_chips_layout.addWidget(self.btn_s_running)
        s_chips_layout.addWidget(self.btn_s_stopped)
        s_chips_layout.addStretch()
        systemd_layout.addLayout(s_chips_layout)
        
        systemd_scroll = QScrollArea()
        systemd_scroll.setWidgetResizable(True)
        systemd_scroll.setFrameShape(QFrame.Shape.NoFrame)
        systemd_scroll.setStyleSheet("background: transparent;")
        
        self.lbl_systemd_alerts = QLabel("Checking systemd --failed...")
        self.lbl_systemd_alerts.setProperty("class", "TabMonospace")
        self.lbl_systemd_alerts.setStyleSheet("color: #fca5a5; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 16px;")
        self.lbl_systemd_alerts.setWordWrap(True)
        self.lbl_systemd_alerts.setAlignment(Qt.AlignmentFlag.AlignTop)
        
        systemd_scroll.setWidget(self.lbl_systemd_alerts)
        systemd_layout.addWidget(systemd_scroll)
        self.alerts_tabs.addTab(systemd_widget, "Systemd")
        
        al_layout.addWidget(self.alerts_tabs)
        col2.addWidget(alerts_card)
        
        # Add Columns to Bento Grid
        grid_layout.addLayout(col1, 3)
        grid_layout.addLayout(col2, 3)
        grid_layout.addLayout(col3, 4)
        
        main_layout.addLayout(grid_layout)
        
        self.resize(1200, 750)
        self.render_sync_pairs()

    def launch_updates(self):
        terminals = [
            "kitty -e yay -Syu",
            "alacritty -e yay -Syu",
            "foot -e yay -Syu",
            "xterm -e yay -Syu"
        ]
        term_cmd = " || ".join(terminals)
        subprocess.Popen(term_cmd, shell=True, env=ENV)

    def render_sync_pairs(self):
        for i in reversed(range(self.sync_grid.count())): 
            widgetToRemove = self.sync_grid.itemAt(i).widget()
            self.sync_grid.removeWidget(widgetToRemove)
            if widgetToRemove: widgetToRemove.setParent(None)
            
        sync_file = os.path.expanduser("~/.config/filen-cli/syncPairs.json")
        if not os.path.exists(sync_file):
            lbl = QLabel("No sync pairs configured or filen-cli not installed.")
            lbl.setStyleSheet("color: #94a3b8; font-family: 'Inter', sans-serif;")
            self.sync_grid.addWidget(lbl, 0, 0)
            return
            
        try:
            with open(sync_file, "r") as f:
                pairs = json.load(f)
                
            if not pairs:
                lbl = QLabel("Sync pairs JSON is empty.")
                lbl.setStyleSheet("color: #94a3b8;")
                self.sync_grid.addWidget(lbl, 0, 0)
                return

            headers = ["Alias", "Local Path", "Sync Mode"]
            for col, h in enumerate(headers):
                lbl = QLabel(h)
                lbl.setStyleSheet("font-weight: bold; color: #f8fafc; font-family: 'Inter', sans-serif; padding-bottom: 4px; border-bottom: 1px solid rgba(255,255,255,0.1); font-size: 11px;")
                self.sync_grid.addWidget(lbl, 0, col)

            for row, p in enumerate(pairs, start=1):
                alias = QLabel(str(p.get("alias", "N/A")))
                alias.setStyleSheet("color: #3b82f6; font-weight: bold; font-family: 'Fira Code', monospace; font-size: 9pt;")
                
                # Truncate and parse paths to fit
                l_path = str(p.get("local", "")).replace("/home/bruno", "~")
                if len(l_path) > 18: l_path = l_path[:8] + "..." + l_path[-7:]
                local = QLabel(l_path)
                local.setStyleSheet("color: #94a3b8; font-family: 'Fira Code', monospace; font-size: 9pt;")
                
                mode_str = str(p.get("syncMode", ""))
                mode = QLabel(mode_str)
                mode_color = "#10b981" if "two" in mode_str.lower() else "#f59e0b"
                mode.setStyleSheet(f"color: {mode_color}; font-family: 'Fira Code', monospace; font-size: 9pt;")

                self.sync_grid.addWidget(alias, row, 0)
                self.sync_grid.addWidget(local, row, 1)
                self.sync_grid.addWidget(mode, row, 2)

        except Exception as e:
            lbl = QLabel(f"Error parsing syncPairs.json: {e}")
            lbl.setStyleSheet("color: #ef4444;")
            self.sync_grid.addWidget(lbl, 0, 0)

    def set_kernel_filter(self, mode):
        self.btn_k_crit.setProperty("class", "ChipActive" if mode == "critical" else "ChipButton")
        self.btn_k_full.setProperty("class", "ChipActive" if mode == "full" else "ChipButton")
        self.btn_k_crit.style().unpolish(self.btn_k_crit)
        self.btn_k_crit.style().polish(self.btn_k_crit)
        self.btn_k_full.style().unpolish(self.btn_k_full)
        self.btn_k_full.style().polish(self.btn_k_full)
        
        if mode == "critical":
            self.kernel_cmd = "journalctl -p 3 -n 20 --no-hostname --no-pager"
        else:
            self.kernel_cmd = "journalctl -n 50 --no-hostname --no-pager"
            
        self.lbl_kernel_alerts.setText("Refreshing kernel logs...")
        self.run_async("alerts_kernel", self.kernel_cmd)

    def set_systemd_filter(self, mode):
        self.systemd_mode = mode
        self.btn_s_broken.setProperty("class", "ChipActive" if mode == "broken" else "ChipButton")
        self.btn_s_running.setProperty("class", "ChipActive" if mode == "running" else "ChipButton")
        self.btn_s_stopped.setProperty("class", "ChipActive" if mode == "stopped" else "ChipButton")
        
        for btn in [self.btn_s_broken, self.btn_s_running, self.btn_s_stopped]:
            btn.style().unpolish(btn)
            btn.style().polish(btn)
            
        if mode == "broken":
            sys_cmd = "systemctl list-units --state=failed --no-legend --no-pager"
            usr_cmd = "systemctl --user list-units --state=failed --no-legend --no-pager"
        elif mode == "running":
            sys_cmd = "systemctl list-units --type=service --state=running --no-legend --no-pager"
            usr_cmd = "systemctl --user list-units --type=service --state=running --no-legend --no-pager"
        else:
            sys_cmd = "systemctl list-units --type=service --state=exited --no-legend --no-pager"
            usr_cmd = "systemctl --user list-units --type=service --state=exited --no-legend --no-pager"

        cmd = (
            f'printf "── System ──\n" && {{ {sys_cmd}; }} ; '
            f'printf "── User ──\n" && {{ {usr_cmd}; }}'
        )
        self.lbl_systemd_alerts.setText("Refreshing systemd logs...")
        self.run_async("alerts_systemd", cmd)

    def on_alerts_tab_changed(self, index):
        if index == 0:
            self.set_kernel_filter("critical")
        else:
            self.set_systemd_filter("broken")

    def setup_timers(self):
        self.fast_timer = QTimer(self)
        self.fast_timer.timeout.connect(self.check_thermal)
        self.fast_timer.start(3000)
        
        self.services_timer = QTimer(self)
        self.services_timer.timeout.connect(self.check_services)
        self.services_timer.start(10000)
        
        self.updates_timer = QTimer(self)
        self.updates_timer.timeout.connect(self.check_updates)
        self.updates_timer.start(3600000)
        
        self.filen_timer = QTimer(self)
        self.filen_timer.timeout.connect(self.check_filen_status)
        self.filen_timer.start(30000)

    def check_filen_status(self):
        if self.thermal_critical: return
        self.run_async("filen_status", "systemctl --user is-active filen.timer")

    def run_async(self, identifier, cmd):
        thread = WorkerThread(identifier, cmd)
        thread.result_ready.connect(self.handle_result)
        self.threads.append(thread)
        thread.finished.connect(lambda t=thread: self.threads.remove(t) if t in self.threads else None)
        thread.start()

    def check_thermal(self):
        self.run_async("thermal", r"sensors | grep -i 'Package id 0\|Tctl\|CPU' | head -n 1")
        
        if psutil:
            cpu = psutil.cpu_percent()
            self.lbl_hdr_cpu.setText(f"CPU: {cpu}%")
            
            mem = psutil.virtual_memory()
            used_gb = round(mem.used / (1024**3), 1)
            total_gb = round(mem.total / (1024**3), 1)
            self.lbl_hdr_ram.setText(f"RAM: {used_gb}/{total_gb}GB")
            
            curr_io = psutil.net_io_counters()
            if self.last_net_io:
                up_kbs = round((curr_io.bytes_sent - self.last_net_io.bytes_sent) / 1024 / 3)
                dn_kbs = round((curr_io.bytes_recv - self.last_net_io.bytes_recv) / 1024 / 3)
                self.lbl_hdr_net.setText(f"Net: ↓{dn_kbs} ↑{up_kbs} KB/s")
            self.last_net_io = curr_io

    def check_services(self):
        if self.thermal_critical: return
        self.run_async("filen", "systemctl --user list-timers filen.timer --no-pager")
        self.run_async("filen_logs", "journalctl --user -u filen.service -n 5 --no-pager")
        self.run_async("workspaces", "hyprctl clients -j")
        self.run_async("alerts_kernel", self.kernel_cmd)
        self.set_systemd_filter(self.systemd_mode)
        self.run_async("battery", "upower -i $(upower -e | grep BAT | head -n 1) 2>/dev/null")
        self.run_async("disk", "df -h / | awk 'NR==2 {print $4, $2, $5}'")
        self.run_async("sys_serv", "systemctl is-active dbus NetworkManager bluetooth")
        self.run_async("net_sock", "ss -tap | awk 'NR>1 {print $6}' | awk -F',' '{print $1}' | grep -o '\"[^\"]*\"' | tr -d '\"' | sort | uniq -c | sort -nr | head -n 3")

    def check_updates(self):
        if self.thermal_critical: return
        self.run_async("pacman", "checkupdates | wc -l")
        self.run_async("aur", "yay -Qua | wc -l")
        self.run_async("filen_quota", "filen statfs || echo 'N/A'")

    def handle_result(self, identifier, result):
        if identifier == "thermal":
            match = re.search(r'\+?([0-9.]+)[°][CF]', result)
            temp_str = match.group(0) if match else "N/A"
            self.lbl_hw_temp.setText(f"TMP: {temp_str}")
            
            if match:
                temp = float(match.group(1))
                if temp > 85.0:
                    if not self.thermal_critical:
                        self.thermal_critical = True
                        self.central_widget.setStyleSheet(self.critical_style)
                        self.lbl_kernel_alerts.setText(f"!! THERMAL WARNING !! CPU at {temp}°C.\nPolling suspended.")
                        self.services_timer.stop()
                        self.updates_timer.stop()
                else:
                    if self.thermal_critical:
                        self.thermal_critical = False
                        self.central_widget.setStyleSheet(self.normal_style)
                        self.services_timer.start(10000)
                        self.updates_timer.start(3600000)
                        self.check_services()
                        
        elif identifier == "pacman":
            cnt = result if result.isdigit() else "0"
            self.lbl_pacman.setText(cnt)
            
        elif identifier == "aur":
            cnt = result if result.isdigit() else "0"
            self.lbl_aur.setText(cnt)
            
        elif identifier == "filen":
            lines = result.split('\n')
            timer_info = "Waiting..."
            for line in lines:
                if "filen.timer" in line:
                    match = re.search(r'(\d+[a-zA-Z\s]+left)', line)
                    if match:
                        timer_info = match.group(1).strip()
                    else:
                        timer_info = "Timer active"
                    break
            self.lbl_filen.setText(timer_info)
            
        elif identifier == "filen_status":
            if result.strip() == "active":
                self.lbl_filen_status_dot.setStyleSheet("color: #10b981; font-size: 16px;") # Green
            else:
                self.lbl_filen_status_dot.setStyleSheet("color: #ef4444; font-size: 16px;") # Red
            
        elif identifier == "filen_logs":
            if not result:
                self.filen_logs_lbl.setText("No recent logs found.")
            else:
                filtered_lines = []
                for line in result.split('\n'):
                    # Strip 'archlinux process[123]: ' -> keep timestamp + message
                    msg = re.sub(r'^(.{15})\s+\S+\s+[^:]+:\s*', r'\1 ', line)
                    filtered_lines.append(msg)
                self.filen_logs_lbl.setText("\n".join(filtered_lines))
            
        elif identifier == "filen_quota":
            used_match = re.search(r'Used:\s*([0-9.]+)', result)
            max_match = re.search(r'Max:\s*([0-9.]+)', result)
            
            if used_match and max_match:
                try:
                    used = float(used_match.group(1))
                    total = float(max_match.group(1))
                    if total > 0:
                        pct = int((used / total) * 100)
                        self.quota_bar.setValue(pct)
                        self.lbl_quota_pct.setText(f"{pct}%")
                        self.lbl_quota_text.setText(f"Filen Cloud Quota - {used} / {total} GiB")
                        return
                except:
                    pass
            self.lbl_quota_text.setText(f"Filen Quota: {result}")
            
        elif identifier == "workspaces":
            try:
                clients = json.loads(result)
                workspaces = {}
                for c in clients:
                    ws = str(c.get('workspace', {}).get('id', '?'))
                    cls = c.get('class', 'Unknown')
                    if ws not in workspaces:
                        workspaces[ws] = []
                    workspaces[ws].append(cls)
                
                out = ""
                for ws_id in sorted(workspaces.keys()):
                    out += f"WS {ws_id}: {', '.join(workspaces[ws_id][:2])}\n"
                
                if not out: out = "No active windows"
                self.lbl_workspaces.setText(out)
            except:
                self.lbl_workspaces.setText("Error parsing workspaces")
                
        elif identifier == "alerts_kernel":
            if not result or "-- No entries --" in result:
                txt = "System Healthy\nNo parsed logs to display."
                self.lbl_kernel_alerts.setText(txt)
                self.lbl_kernel_alerts.setStyleSheet("color: #10b981; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 12px;") # Green
            else:
                filtered_lines = []
                for line in result.split('\n'):
                    if "Logs begin at" in line: continue
                    parts = re.split(r'\s\S+\s[^:]+:\s|\skernel:\s', line, maxsplit=1)
                    msg = parts[-1].strip() if len(parts) > 1 else line.strip()
                    msg = re.sub(r'0x[a-fA-F0-9]+\b|\[<[a-fA-F0-9]+>\]', '', msg).strip()
                    
                    if len(msg) > 75: msg = msg[:72] + "..."
                    if msg: filtered_lines.append(msg)
                
                if not filtered_lines:
                    self.lbl_kernel_alerts.setText("System Healthy\n(Errors filtered out)")
                    self.lbl_kernel_alerts.setStyleSheet("color: #10b981; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 12px;") # Green
                else:
                    color = "#fca5a5" if self.btn_k_crit.property("class") == "ChipActive" else "#94a3b8"
                    self.lbl_kernel_alerts.setText("\n".join(filtered_lines))
                    self.lbl_kernel_alerts.setStyleSheet(f"color: {color}; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 12px;")

        elif identifier == "alerts_systemd":
            # Strip header lines to check if there is any actual content
            meaningful_lines = [
                l for l in result.splitlines()
                if l.strip() and not l.startswith("──") and "0 loaded units listed" not in l
            ]
            if not result or not meaningful_lines:
                self.lbl_systemd_alerts.setText("✔ All Services Running.")
                self.lbl_systemd_alerts.setStyleSheet("color: #10b981; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 12px;")
            else:
                self.lbl_systemd_alerts.setText(result)
                self.lbl_systemd_alerts.setStyleSheet("color: #fca5a5; font-family: 'Fira Code', monospace; font-size: 10pt; padding: 12px;")
                
        elif identifier == "battery":
            if result:
                pct = "N/A"
                for line in result.split('\n'):
                    line = line.strip()
                    if line.startswith('percentage:'):
                        pct = line.split(':')[1].strip()
                
                self.lbl_bat_pct.setText(f"BAT: {pct}")
            else:
                self.lbl_bat_pct.setText("BAT: AC")
                
        elif identifier == "disk":
            parts = result.split()
            if len(parts) >= 3:
                pct_str = parts[2].replace('%', '')
                if pct_str.isdigit():
                    pct = int(pct_str)
                    self.disk_bar.setValue(pct)
                    self.lbl_disk_pct.setText(f"{pct}%")
                self.lbl_disk_title.setText(f"Root (/) - {parts[0]} Free")

        elif identifier == "sys_serv":
            lines = result.split()
            if len(lines) >= 3:
                dbus = "🟢" if lines[0] == "active" else "🔴"
                nm = "🟢" if lines[1] == "active" else "🔴"
                bt = "🟢" if lines[2] == "active" else "🔴"
                self.lbl_sys_services.setText(f"dbus: {dbus} | NetMgr: {nm} | blue: {bt}")
                
        elif identifier == "net_sock":
            if result:
                procs = []
                for line in result.split('\n'):
                    parts = line.strip().split(maxsplit=1)
                    if len(parts) == 2:
                        procs.append(f"{parts[1][:15]}: {parts[0]}")
                self.lbl_net_sockets.setText("\n".join(procs))
            else:
                self.lbl_net_sockets.setText("Idle")

if __name__ == "__main__":
    QApplication.setApplicationName("system-dashboard")
    QApplication.setDesktopFileName("")
    app = QApplication(sys.argv)
    window = DashboardWindow()
    window.show()
    sys.exit(app.exec())
