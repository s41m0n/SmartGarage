package mainPackage;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.DefaultCaret;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;

public class GUI extends JFrame{

    private final JPanel mainPanel = new JPanel();
    private final JButton openBtn = new JButton("Open Garage");
    private final JButton stopBtn = new JButton("Stop Car");
    private final JTextPane outputArea = new JTextPane();
    private final JScrollPane outputPanel = new JScrollPane(this.outputArea, 
            JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
    private final JLabel title = new JLabel("Simone Magnani's Smart Garage");
    private SerialMonitor monitor;
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    
    public GUI() {
        super("Smart Garage v1.0");
        this.openBtn.addActionListener(e->{
            this.monitor.sendMessage("open");
        });
        this.stopBtn.addActionListener(e-> {
            this.monitor.sendMessage("stop");
        });
        this.title.setFont(new Font("Arial", Font.BOLD, 25));
        this.openBtn.setFont(new Font("Arial", Font.BOLD, 16));
        this.stopBtn.setFont(new Font("Arial", Font.BOLD, 16));
        this.outputArea.setFont(new Font("Arial", Font.BOLD, 12));
        
        StyledDocument doc = this.outputArea.getStyledDocument();
        SimpleAttributeSet center = new SimpleAttributeSet();
        StyleConstants.setAlignment(center, StyleConstants.ALIGN_CENTER);
        doc.setParagraphAttributes(0, doc.getLength(), center, false);
        
        this.outputArea.setEditable(false);
        DefaultCaret c = (DefaultCaret)this.outputArea.getCaret();
        c.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);
        
        this.mainPanel.setLayout(new BorderLayout());
        title.setHorizontalAlignment(JLabel.CENTER);
        title.setVerticalAlignment(JLabel.CENTER);
        
        JPanel southPanel = new JPanel();
        southPanel.add(this.openBtn);
        southPanel.add(this.stopBtn);
        
        this.mainPanel.add(this.title, BorderLayout.NORTH);
        this.mainPanel.add(this.outputPanel, BorderLayout.CENTER);
        this.mainPanel.add(southPanel, BorderLayout.SOUTH);

        this.getRootPane().setBorder(BorderFactory.createMatteBorder(4, 4, 4, 4, Color.BLACK));
        this.add(this.mainPanel);
        setSize(400,600);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    protected void init(){
        setVisible(true);
    }
    
    public void registerMonitor(SerialMonitor monitor) {
        this.monitor = monitor;
    }
    
    protected void notifyNewMessage(String message) {
        this.outputArea.setText(this.outputArea.getText() + message);
        this.outputPanel.revalidate();
    }
}
