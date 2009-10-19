
package jpocket.gui.workers;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */



import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingWorker;
import javax.swing.SwingWorker.StateValue;

/**
 *
 * @author vince
 */
public class WorkerProgressFrame extends JDialog
        implements ActionListener, PropertyChangeListener
{
    public static final String PROGRESS_PROP        = "progress" ;
    public static final String INFO_LABEL_PROP      = "infoLabel" ;
    public static final String WARN_PROP            = "warn" ;
    public static final String ERR_PROP             = "err" ;
    public static final String LOG_PROP             = "log" ;

    // Components
    private JProgressBar progressBar;
    private JButton cancelButton, okButton;
    private JTextArea workerOutput;
    private JLabel infoLabel ;

    private SwingWorker worker ;

    private int nErr ;
    private int nWarn ;

    public WorkerProgressFrame (SwingWorker worker,
                                String title,
                                Point location)
    {
		super();

        setTitle(title);
        
        this.worker = worker ;
        
        nErr = 0 ;
        nWarn = 0 ;
        
        initComponents() ;

        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        setLocation(location);
        setVisible(true) ;
        setSize(new Dimension(350, 200));
        
        pack() ;
    }

    public void startWorker() {
        nErr = 0 ;
        nWarn = 0 ;
        worker.execute();
    }
    
    public void setProgressIndeterminate(boolean undetermined) {
        this.progressBar.setIndeterminate(undetermined);
    }

    private void initComponents()
    {
        cancelButton = new JButton("Cancel");
        cancelButton.setActionCommand("cancel");
        cancelButton.addActionListener(this);

        okButton = new JButton("OK");
        okButton.setActionCommand("ok");
        okButton.addActionListener(this);
        okButton.setEnabled(false) ;

        JPanel p = new JPanel() ;
        p.setLayout(new GridLayout(1, 2)) ;
        p.add(okButton) ;
        p.add(cancelButton) ;

        progressBar = new JProgressBar(0, 100);
        progressBar.setValue(0);
        progressBar.setStringPainted(true);

        workerOutput = new JTextArea(15, 50);
        workerOutput.setMargin(new Insets(5,5,5,5));
        workerOutput.setEditable(false);
        workerOutput.setCursor(null);
        workerOutput.setTabSize(3) ;

        infoLabel = new JLabel("Awaiting...") ;

        JPanel p2 = new JPanel() ;
        p2.setLayout(new BoxLayout(p2, BoxLayout.Y_AXIS)) ;
        p2.add(new JScrollPane(workerOutput), BorderLayout.NORTH) ;
        p2.add(infoLabel) ;

        add(progressBar, BorderLayout.NORTH);
        add(p2);
        add(p, BorderLayout.SOUTH);
    }

    @Override
    public void actionPerformed(ActionEvent e)
    {
        if(e.getActionCommand().equals("ok")) {
            dispose() ;
        }
        else {
            worker.cancel(false);
            dispose() ;
        }
    }

    @Override
    public void propertyChange(PropertyChangeEvent evt)
    {
        if(INFO_LABEL_PROP.equals(evt.getPropertyName())) {
            infoLabel.setText(evt.getNewValue().toString());
        }
        else if (PROGRESS_PROP.equals(evt.getPropertyName())) {
            Integer p = (Integer) evt.getNewValue() ;
            progressBar.setValue((Integer) evt.getNewValue());
            if(p.intValue() == 100) {
                okButton.setEnabled(true) ;
                progressBar.setIndeterminate(false);
            }
        }
        else if(LOG_PROP.equals(evt.getPropertyName())) {
            String message = evt.getNewValue().toString() ;
            message = message.replace("\n", "\n-I-\t") ;
            
            workerOutput.append(message);
        }
        else if(ERR_PROP.equals(evt.getPropertyName())) {
            nErr++ ;

            String message = evt.getNewValue().toString() ;
            message = message.replace("\n", "\n-E-\t") ;

            workerOutput.append(message);
        }
        else if(WARN_PROP.equals(evt.getPropertyName())) {
            nWarn++ ;

            String message = evt.getNewValue().toString() ;
            message = message.replace("\n", "\n-W-\t") ;
            
            workerOutput.append(message);
        }
        else if(StateValue.DONE.equals(evt.getNewValue())) {
            progressBar.setIndeterminate(false);
            progressBar.setValue(100);
            okButton.setEnabled(true) ;
            
        }
    }

    public int getNErr() {
        return nErr;
    }

    public void setNErr(int nErr) {
        this.nErr = nErr;
    }

    public int getNWarn() {
        return nWarn;
    }

    public void setNWarn(int nWarn) {
        this.nWarn = nWarn;
    }

    public void setWorker(SwingWorker worker) {
        this.worker = worker;
        this.progressBar.setValue(0);
    }
    
}

