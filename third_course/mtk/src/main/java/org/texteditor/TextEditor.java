package org.texteditor;
import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.io.*;

public class TextEditor extends JFrame {
    private JTextArea textArea;
    private JTextArea statusArea;
    private JFileChooser fileChooser;
    private JComboBox<String> fontSizeComboBox;
    private File currentFile;

    public TextEditor() {
        setTitle("Java Text Editor");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);

        // Set modern Windows look and feel
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
            e.printStackTrace();
        }

        initComponents();
        layoutComponents();

        setVisible(true);
    }

    private void initComponents() {
        textArea = new JTextArea();
        textArea.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        textArea.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

        statusArea = new JTextArea(3, 1);
        statusArea.setEditable(false);
        statusArea.setBackground(new Color(240, 240, 240));
        statusArea.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createEmptyBorder(5, 5, 5, 5),
                BorderFactory.createEtchedBorder()
        ));
        statusArea.setFont(new Font("Segoe UI", Font.PLAIN, 12));

        fileChooser = new JFileChooser();

        // Font size combo box with predefined values
        String[] fontSizes = {"8", "10", "12", "14", "16", "18", "20", "22", "24", "28", "32", "36"};
        fontSizeComboBox = new JComboBox<>(fontSizes);
        fontSizeComboBox.setSelectedItem("12");
        fontSizeComboBox.addActionListener(e -> {
            String selectedSize = (String) fontSizeComboBox.getSelectedItem();
            try {
                int size = Integer.parseInt(selectedSize);
                Font currentFont = textArea.getFont();
                textArea.setFont(new Font(currentFont.getName(), currentFont.getStyle(), size));

                // Also update status area font size but keep it smaller
                Font statusFont = statusArea.getFont();
                statusArea.setFont(new Font(statusFont.getName(), statusFont.getStyle(), Math.max(10, size - 2)));
            } catch (NumberFormatException ex) {
                showError("Invalid font size: " + selectedSize);
            }
        });
    }

    private void layoutComponents() {
        // Create menu bar with modern styling
        JMenuBar menuBar = new JMenuBar();
        menuBar.setBackground(new Color(240, 240, 240));

        // File menu
        JMenu fileMenu = new JMenu("File");
        fileMenu.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        JMenuItem newMenuItem = new JMenuItem("New");
        JMenuItem openMenuItem = new JMenuItem("Open");
        JMenuItem saveMenuItem = new JMenuItem("Save");
        JMenuItem saveAsMenuItem = new JMenuItem("Save As");
        JMenuItem exitMenuItem = new JMenuItem("Exit");

        // Set modern font for menu items
        Font menuFont = new Font("Segoe UI", Font.PLAIN, 12);
        newMenuItem.setFont(menuFont);
        openMenuItem.setFont(menuFont);
        saveMenuItem.setFont(menuFont);
        saveAsMenuItem.setFont(menuFont);
        exitMenuItem.setFont(menuFont);

        newMenuItem.addActionListener(e -> newFile());
        openMenuItem.addActionListener(e -> openFile());
        saveMenuItem.addActionListener(e -> saveFile());
        saveAsMenuItem.addActionListener(e -> saveAsFile());
        exitMenuItem.addActionListener(e -> System.exit(0));

        fileMenu.add(newMenuItem);
        fileMenu.add(openMenuItem);
        fileMenu.add(saveMenuItem);
        fileMenu.add(saveAsMenuItem);
        fileMenu.addSeparator();
        fileMenu.add(exitMenuItem);

        // Edit menu
        JMenu editMenu = new JMenu("Edit");
        editMenu.setFont(menuFont);
        JMenuItem runMenuItem = new JMenuItem("Run");
        runMenuItem.setFont(menuFont);
        runMenuItem.addActionListener(e -> runCode());
        editMenu.add(runMenuItem);

        // Help menu
        JMenu helpMenu = new JMenu("Help");
        helpMenu.setFont(menuFont);

        menuBar.add(fileMenu);
        menuBar.add(editMenu);
        menuBar.add(helpMenu);
        setJMenuBar(menuBar);

        // Create modern toolbar
        JToolBar toolBar = new JToolBar();
        toolBar.setFloatable(false);
        toolBar.setBackground(new Color(240, 240, 240));
        toolBar.setBorder(BorderFactory.createMatteBorder(0, 0, 1, 0, new Color(220, 220, 220)));

        JButton newButton = new JButton("New");
        JButton openButton = new JButton("Open");
        JButton saveButton = new JButton("Save");
        JButton runButton = new JButton("Run");

        // Style toolbar buttons
        for (JButton button : new JButton[]{newButton, openButton, saveButton, runButton}) {
            button.setFont(menuFont);
            button.setBackground(Color.WHITE);
            button.setBorder(BorderFactory.createCompoundBorder(
                    BorderFactory.createLineBorder(new Color(220, 220, 220)),
                    BorderFactory.createEmptyBorder(5, 10, 5, 10)
            ));
            button.setFocusPainted(false);
        }

        newButton.addActionListener(e -> newFile());
        openButton.addActionListener(e -> openFile());
        saveButton.addActionListener(e -> saveFile());
        runButton.addActionListener(e -> runCode());

        // Add buttons with font size near Save button
        toolBar.add(newButton);
        toolBar.add(openButton);
        toolBar.add(saveButton);

        // Add font size selector right after the Save button
        toolBar.add(Box.createHorizontalStrut(10));
        toolBar.add(new JLabel("Font Size: "));
        toolBar.add(fontSizeComboBox);

        // Add glue to push Run button to the right
        toolBar.add(Box.createHorizontalGlue());
        toolBar.add(runButton);

        // Main content with split pane
        JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        splitPane.setTopComponent(new JScrollPane(textArea));
        splitPane.setBottomComponent(new JScrollPane(statusArea));
        splitPane.setDividerLocation(450);
        splitPane.setResizeWeight(0.8);
        splitPane.setBorder(BorderFactory.createEmptyBorder());

        JPanel mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(toolBar, BorderLayout.NORTH);
        mainPanel.add(splitPane, BorderLayout.CENTER);

        add(mainPanel);
    }

    private void newFile() {
        if (confirmSave()) {
            textArea.setText("");
            currentFile = null;
            setTitle("Java Text Editor - New File");
        }
    }

    private void openFile() {
        if (confirmSave()) {
            int returnVal = fileChooser.showOpenDialog(this);
            if (returnVal == JFileChooser.APPROVE_OPTION) {
                currentFile = fileChooser.getSelectedFile();
                try (BufferedReader reader = new BufferedReader(new FileReader(currentFile))) {
                    textArea.read(reader, null);
                    setTitle("Java Text Editor - " + currentFile.getName());
                } catch (IOException ex) {
                    showError("Error reading file: " + ex.getMessage());
                }
            }
        }
    }

    private void saveFile() {
        if (currentFile == null) {
            saveAsFile();
        } else {
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(currentFile))) {
                textArea.write(writer);
            } catch (IOException ex) {
                showError("Error saving file: " + ex.getMessage());
            }
        }
    }

    private void saveAsFile() {
        int returnVal = fileChooser.showSaveDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            currentFile = fileChooser.getSelectedFile();
            setTitle("Java Text Editor - " + currentFile.getName());
            saveFile();
        }
    }

    private boolean confirmSave() {
        if (textArea.getDocument().getLength() > 0) {
            int result = JOptionPane.showConfirmDialog(this,
                    "Do you want to save the current file?", "Confirm Save",
                    JOptionPane.YES_NO_CANCEL_OPTION);

            if (result == JOptionPane.YES_OPTION) {
                saveFile();
                return true;
            }
            else if (result == JOptionPane.NO_OPTION) {
                return true;
            }
            else {
                return false;
            }
        }
        return true;
    }

    private void runCode() {
        // Placeholder for future run functionality
        showError("Run button clicked - functionality not implemented yet");
    }

    private void showError(String message) {
        statusArea.setText(message + "\n");
        statusArea.setCaretPosition(statusArea.getDocument().getLength());
    }
}