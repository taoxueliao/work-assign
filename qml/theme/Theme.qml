pragma Singleton
import QtQuick 2.15
import QtQuick.Window 2.15

QtObject {
    readonly property real scale: Math.max(0.85, Math.min(1.35, Math.min(Screen.width, Screen.desktopAvailableWidth) / 1440))
    readonly property int radius: Math.round(16 * scale)
    readonly property int spacing: Math.round(12 * scale)
    readonly property int motionMs: 280
    readonly property int titleSize: Math.round(22 * scale)
    readonly property int bodySize: Math.round(16 * scale)
    readonly property int buttonTextSize: Math.round(15 * scale)
    readonly property int pageMargin: Math.round(24 * scale)
    readonly property color background: "#F4F1EA"
    readonly property color card: "#FFFFFF"
    readonly property color accent: "#2F6F4E"
    readonly property color text: "#1C1B19"
    readonly property color muted: "#6E6A62"
}
