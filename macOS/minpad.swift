import Cocoa

class AppDelegate: NSObject, NSApplicationDelegate {
    var window: NSWindow!
    var textView: NSTextView!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Create the window
        window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 400, height: 300),
            styleMask: [.titled, .closable, .resizable, .fullSizeContentView],
            backing: .buffered,
            defer: false
        )

        // Set the window title
        window.title = "Minimal Text Editor"

        // Create the text view
        textView = NSTextView(frame: window.contentView!.bounds)
        textView.isEditable = true
        textView.autoresizingMask = [.width, .height]
        window.contentView!.addSubview(textView)

        // Create the menu bar
        let menuBar = NSMenu()
        NSApp.mainMenu = menuBar

        // Create the File menu
        let fileMenu = NSMenu()
        fileMenu.addItem(withTitle: "New", action: #selector(newFile), keyEquivalent: "n")
        fileMenu.addItem(withTitle: "Open...", action: #selector(openFile), keyEquivalent: "o")
        fileMenu.addItem(withTitle: "Save", action: #selector(saveFile), keyEquivalent: "s")
        fileMenu.addItem(withTitle: "Save As...", action: #selector(saveFileAs), keyEquivalent: "S")
        fileMenu.addItem(NSMenuItem.separator())
        fileMenu.addItem(withTitle: "Quit", action: #selector(quit), keyEquivalent: "q")

        let fileMenuItem = NSMenuItem()
        fileMenuItem.submenu = fileMenu
        menuBar.addItem(fileMenuItem)

        // Create the Edit menu
        let editMenu = NSMenu()
        editMenu.addItem(withTitle: "Undo", action: #selector(undo), keyEquivalent: "z")
        editMenu.addItem(withTitle: "Redo", action: #selector(redo), keyEquivalent: "Z")
        editMenu.addItem(NSMenuItem.separator())
        editMenu.addItem(withTitle: "Cut", action: #selector(cut), keyEquivalent: "x")
        editMenu.addItem(withTitle: "Copy", action: #selector(customCopy), keyEquivalent: "c")
        editMenu.addItem(withTitle: "Paste", action: #selector(paste), keyEquivalent: "v")

        let editMenuItem = NSMenuItem()
        editMenuItem.submenu = editMenu
        menuBar.addItem(editMenuItem)

        // Show the window
        window.makeKeyAndOrderFront(nil)
    }

    @objc func newFile() {
        // Handle new file action
    }

    @objc func openFile() {
        // Handle open file action
    }

    @objc func saveFile() {
        // Handle save file action
    }

    @objc func saveFileAs() {
        // Handle save file as action
    }

    @objc func quit() {
        NSApp.terminate(nil)
    }

    @objc func undo() {
        textView.undoManager?.undo()
    }

    @objc func redo() {
        textView.undoManager?.redo()
    }

    @objc func cut() {
        textView.cut(nil)
    }

    @objc func customCopy() {
        textView.copy(nil)
    }

    @objc func paste() {
        textView.paste(nil)
    }
}

let appDelegate = AppDelegate()
NSApplication.shared.delegate = appDelegate
_ = NSApplicationMain(CommandLine.argc, CommandLine.unsafeArgv)
