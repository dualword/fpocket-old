/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jpocket.jmol;
/* $RCSfile$
 * $Author: hansonr $
 * $Date: 2009-06-26 23:35:44 -0500 (Fri, 26 Jun 2009) $
 * $Revision: 11131 $
 *
 * Copyright (C) 2000-2005  The Jmol Development Team
 *
 * Contact: jmol-developers@lists.sf.net
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 */
import org.jmol.api.*;
import org.jmol.console.ScriptEditor;
import org.jmol.export.dialog.Dialog;
import org.jmol.export.history.HistoryFile;
import org.jmol.popup.JmolPopup;
import org.jmol.i18n.GT;
import org.jmol.util.*;
import org.jmol.viewer.JmolConstants;
import org.jmol.viewer.ScriptContext;
import org.jmol.viewer.Viewer;
import org.openscience.jmol.app.*;

import java.awt.*;
import java.awt.dnd.DropTarget;
import java.awt.event.*;
import java.beans.*;
import java.io.*;
import java.lang.reflect.Method;
import java.net.URI;
import java.util.*;

import javax.swing.*;

import org.jmol.export.image.ImageCreator;
import org.openscience.jmol.app.jmolpanel.AppConsole;
import org.openscience.jmol.app.jmolpanel.AtomSetChooser;
import org.openscience.jmol.app.jmolpanel.ConsoleTextArea;
import org.openscience.jmol.app.jmolpanel.FileDropper;
import org.openscience.jmol.app.jmolpanel.GaussianDialog;
import org.openscience.jmol.app.jmolpanel.MeasurementTable;
import org.openscience.jmol.app.jmolpanel.PreferencesDialog;

public class JMolPanel extends JPanel {

  /**
   * The data model.
   */

  public JmolViewer viewer;
  JmolAdapter modelAdapter;
  JmolApp jmolApp;


  GuiMap guimap = new GuiMap();
  
  JMolDisplayPanel display;
  StatusBar status;
  protected GaussianDialog gaussianDialog;
  private PreferencesDialog preferencesDialog;
  MeasurementTable measurementTable;
  //private JMenu recentFilesMenu;
  public AppConsole appConsole;
  public ScriptEditor scriptEditor;
  public AtomSetChooser atomSetChooser;
  private ExecuteScriptAction executeScriptAction;
  protected JFrame frame;

  JmolPopup jmolpopup;
  String language;

  // private CDKPluginManager pluginManager;
  private static int numWindows = 0;
  private static Dimension screenSize = null;
  int startupWidth, startupHeight;

  PropertyChangeSupport pcs = new PropertyChangeSupport(this);

  // Window names for the history file
  private final static String CONSOLE_WINDOW_NAME = "Console";
  private final static String EDITOR_WINDOW_NAME = "ScriptEditor";
  private final static String SCRIPT_WINDOW_NAME = "ScriptWindow";
  private final static String FILE_OPEN_WINDOW_NAME = "FileOpen";
  private final static String WEB_MAKER_WINDOW_NAME = "JmolWebPageMaker";


  /**
   * Button group for toggle buttons in the toolbar.
   */

  protected SplashInterface splashToDelete;

  protected JFrame consoleframe;

  String appletContext;

  String menuStructure;
  String menuFile;

  static HistoryFile historyFile;

  public JMolPanel(JmolApp jmolApp, JFrame frame,
                    int startupWidth, int startupHeight, String commandOptions, Point loc) {
    super(true);
    this.jmolApp = jmolApp;
    this.frame = frame;
    this.startupWidth = startupWidth;
    this.startupHeight = startupHeight;
    historyFile = jmolApp.historyFile;

    numWindows++;

    try {
      say("history file is " + historyFile.getFile().getAbsolutePath());
    } catch (Exception e) {
    }

    setBorder(BorderFactory.createEtchedBorder());
    setLayout(new BorderLayout());
    language = GT.getLanguage();

    status = (StatusBar) createStatusBar();
    say(GT._("Initializing 3D display..."));

    display = new JMolDisplayPanel(this);
    viewer = JmolViewer.allocateViewer(display, modelAdapter, null, null, null,
    appletContext = commandOptions, new MyStatusListener());
//    if (jmolApp.haveDisplay)
      display.setViewer(viewer);
  //  else
    //  viewer.setScreenDimension(new Dimension(jmolApp.startupWidth,
      //    jmolApp.startupHeight));

    if (!jmolApp.haveDisplay)
      return;
    say(GT._("Initializing Preferences..."));
    //preferencesDialog = new PreferencesDialog(this, frame, guimap, viewer);
    say(GT._("Initializing Recent Files..."));
    //recentFiles = new RecentFilesDialog(frame);
    say(GT._("Initializing Script Window..."));
    appConsole = new AppConsole(viewer, frame);
    say(GT._("Initializing Script Editor..."));
    scriptEditor = new ScriptEditor(viewer, frame, appConsole);
//    appConsole.setScriptEditor(scriptEditor);
    say(GT._("Initializing Measurements..."));
    measurementTable = new MeasurementTable(viewer, frame);

    // install the command table
    say(GT._("Building Command Hooks..."));
    commands = new Hashtable();
    if (display != null) {
      Action[] actions = getActions();
      for (int i = 0; i < actions.length; i++) {
        Action a = actions[i];
        commands.put(a.getValue(Action.NAME), a);
      }
    }

    menuItems = new Hashtable();
    say(GT._("Building Menubar..."));
    executeScriptAction = new ExecuteScriptAction();

    add("Center", display);
    add("South", status);

    say(GT._("Starting display..."));
    display.start();

    if (menuFile != null) {
      menuStructure = viewer.getFileAsString(menuFile);
    }
    jmolpopup = JmolPopup.newJmolPopup(viewer, true, menuStructure, true);


    // Repositionning windows
    if (appConsole != null)
      historyFile.repositionWindow(SCRIPT_WINDOW_NAME, appConsole, 200, 100);
    if (scriptEditor != null)
      historyFile.repositionWindow(EDITOR_WINDOW_NAME, scriptEditor, 150, 50);

    say(GT._("Setting up Drag-and-Drop..."));
    FileDropper dropper = new FileDropper();
    final JFrame f = frame;
    dropper.addPropertyChangeListener(new PropertyChangeListener() {
      public void propertyChange(PropertyChangeEvent evt) {
        // System.out.println("Drop triggered...");
        f.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        if (evt.getPropertyName().equals(FileDropper.FD_PROPERTY_FILENAME)) {
          final String filename = evt.getNewValue().toString();
          viewer.openFileAsynchronously(filename);
        } else if (evt.getPropertyName().equals(FileDropper.FD_PROPERTY_INLINE)) {
          final String inline = evt.getNewValue().toString();
          viewer.openStringInline(inline);
        }
        f.setCursor(Cursor.getDefaultCursor());
      }
    });

    this.setDropTarget(new DropTarget(this, dropper));
    this.setEnabled(true);

    say(GT._("Launching main frame..."));
  }

  protected static void startJmol(JmolApp jmolApp) {

    Dialog.setupUIManager();

    // now pass these to viewer

    JMolPanel jmol = getJmol(jmolApp);
    jmolApp.startViewer(jmol.viewer, null);

//    if (jmolApp.haveConsole) {
//      // Adding console frame to grab System.out & System.err
//      jmol.consoleframe = new JFrame(GT._("Jmol Console"));
//      jmol.consoleframe.setIconImage(jmol.frame.getIconImage());
//      try {
//        final ConsoleTextArea consoleTextArea = new ConsoleTextArea(true);
//        consoleTextArea.setFont(java.awt.Font.decode("monospaced"));
//        jmol.consoleframe.getContentPane().add(new JScrollPane(consoleTextArea),
//            java.awt.BorderLayout.CENTER);
//        if (Boolean.getBoolean("clearConsoleButton")) {
//          JButton buttonClear = new JButton(GT._("Clear"));
//          buttonClear.addActionListener(new ActionListener() {
//            public void actionPerformed(ActionEvent e) {
//              consoleTextArea.setText("");
//            }
//          });
//          jmol.consoleframe.getContentPane().add(buttonClear,
//              java.awt.BorderLayout.SOUTH);
//        }
//      } catch (IOException e) {
//        JTextArea errorTextArea = new JTextArea();
//        errorTextArea.setFont(java.awt.Font.decode("monospaced"));
//        jmol.consoleframe.getContentPane().add(new JScrollPane(errorTextArea),
//            java.awt.BorderLayout.CENTER);
//        errorTextArea.append(GT._("Could not create ConsoleTextArea: ") + e);
//      }
//      //setWindow(CONSOLE_WINDOW_NAME, jmol.consoleframe, jmol);
//    }
  }

  public static JMolPanel getJmol(JmolApp jmolApp) {

    String commandOptions = jmolApp.commandOptions;
    try {
      UIManager
          .setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
    } catch (Exception exc) {
      System.err.println("Error loading L&F: " + exc);
    }

    //screenSize = Toolkit.getDefaultToolkit().getScreenSize();

    JMolPanel window = new JMolPanel(jmolApp, null, jmolApp.startupWidth,
                                     jmolApp.startupHeight, commandOptions, null);
    
    return window;
  }

  void report(String str) {
    if (jmolApp.isSilent)
      return;
    Logger.info(str);
  }

  private void say(String message) {
    if (jmolApp.haveDisplay)
      if (splashToDelete == null) {
        report(message);
      } else {
        splashToDelete.showStatus(message);
      }
  }

  /**
   * @return A list of Actions that is understood by the upper level
   * application
   */
  public Action[] getActions() {
    ArrayList actions = new ArrayList();
    actions.addAll(Arrays.asList(display.getActions()));
    return (Action[]) actions.toArray(new Action[0]);
  }

  /**
   * @return The hosting frame, for the file-chooser dialog.
   */
  protected Frame getFrame() {

    for (Container p = getParent(); p != null; p = p.getParent()) {
      if (p instanceof Frame) {
        return (Frame) p;
      }
    }
    return null;
  }

  private static class ActionChangedListener implements PropertyChangeListener {

    AbstractButton button;

    ActionChangedListener(AbstractButton button) {
      super();
      this.button = button;
    }

    public void propertyChange(PropertyChangeEvent e) {

      String propertyName = e.getPropertyName();
      if (e.getPropertyName().equals(Action.NAME)) {
        String text = (String) e.getNewValue();
        if (button.getText() != null) {
          button.setText(text);
        }
      } else if (propertyName.equals("enabled")) {
        Boolean enabledState = (Boolean) e.getNewValue();
        button.setEnabled(enabledState.booleanValue());
      }
    }
  }

  private Hashtable commands;
  private Hashtable menuItems;
  /**
   * Fetch the action that was created for the given
   * command.
   * @param cmd  Name of the action.
   * @return The action
   */
  protected Action getAction(String cmd) {
    return (Action) commands.get(cmd);
  }
  
  /**
   * Take the given string and chop it up into a series
   * of strings on whitespace boundries.  This is useful
   * for trying to get an array of strings out of the
   * resource file.
   * @param input String to chop
   * @return Strings chopped on whitespace boundries
   */
  protected String[] tokenize(String input) {

    Vector v = new Vector();
    StringTokenizer t = new StringTokenizer(input);
    String cmd[];

    while (t.hasMoreTokens()) {
      v.addElement(t.nextToken());
    }
    cmd = new String[v.size()];
    for (int i = 0; i < cmd.length; i++) {
      cmd[i] = (String) v.elementAt(i);
    }

    return cmd;
  }

  protected Component createStatusBar() {
    return new StatusBar();
  }

  public static final String chemFileProperty = "chemFile";

  class MyStatusListener implements JmolStatusListener {

    /* starting with Jmol 11.7.27, JmolStatusListener extends JmolCallbackListener
     *
     * providing a simpler interface if all that is wanted is callback functionality.
     *
     * Only three methods are involved:
     *
     * boolean notifyEnabled(int type)
     *   -- lets the statusManager know if there is an implementation
     *      of a given callback type
     *
     * void notifyCallback(int type, Object[] data)
     *   -- callback action; data varies with callback type
     *   -- see org.jmol.viewer.StatusManager for details
     *
     * void setCallbackFunction(String callbackType, String callbackFunction)
     *   -- called by statusManager in response to the
     *      "set callback" script command
     *   -- also used by the Jmol application to change menus and languages
     *   -- can remain unimplemented if no such user action is intended
     *
     */

    /// JmolCallbackListener interface ///

    public boolean notifyEnabled(int type) {
      switch (type) {
      case JmolConstants.SHOW_EDITOR:
      case JmolConstants.CALLBACK_ANIMFRAME:
      case JmolConstants.CALLBACK_ECHO:
      case JmolConstants.CALLBACK_LOADSTRUCT:
      case JmolConstants.CALLBACK_MEASURE:
      case JmolConstants.CALLBACK_MESSAGE:
      case JmolConstants.CALLBACK_PICK:
      case JmolConstants.CALLBACK_SCRIPT:
        return true;
      case JmolConstants.CALLBACK_ERROR:
      case JmolConstants.CALLBACK_HOVER:
      case JmolConstants.CALLBACK_MINIMIZATION:
      case JmolConstants.CALLBACK_RESIZE:
      case JmolConstants.CALLBACK_SYNC:
      //applet only (but you could change this for your listener)
      }
      return false;
    }

    public void notifyCallback(int type, Object[] data) {
      String strInfo = (data == null || data[1] == null ? null : data[1]
          .toString());
      switch (type) {
      case JmolConstants.SHOW_EDITOR:
        //showEditor(true, strInfo);
        return;
      case JmolConstants.CALLBACK_LOADSTRUCT:
        notifyFileLoaded(strInfo, (String) data[2], (String) data[3],
            (String) data[4]);
        break;
      case JmolConstants.CALLBACK_ANIMFRAME:
        int[] iData = (int[]) data[1];
        notifyFrameChanged(iData[0], iData[1], iData[2]);
        break;
      case JmolConstants.CALLBACK_ECHO:
        sendConsoleEcho(strInfo);
        break;
      case JmolConstants.CALLBACK_MEASURE:
        String status = (String) data[3];
        if (status.indexOf("Picked") >= 0) //picking mode
          notifyAtomPicked(strInfo);
        else if (status.indexOf("Completed") >= 0)
          sendConsoleEcho(strInfo.substring(strInfo.lastIndexOf(",") + 2,
              strInfo.length() - 1));
        if (status.indexOf("Pending") < 0) {
          //System.out.println("jmol callback measure" + status);
          measurementTable.updateTables();
        }
        break;
      case JmolConstants.CALLBACK_MESSAGE:
        sendConsoleMessage(data == null ? null : strInfo);
        break;
      case JmolConstants.CALLBACK_PICK:
        notifyAtomPicked(strInfo);
        break;
      case JmolConstants.CALLBACK_SCRIPT:
        int msWalltime = ((Integer) data[3]).intValue();
        if (appConsole != null) {
          // general message has msWalltime = 0
          // special messages have msWalltime < 0
          // termination message has msWalltime > 0 (1 + msWalltime)
          // "script started"/"pending"/"script terminated"/"script completed"
          //   do not get sent to console
          if (msWalltime == 0) {
            appConsole.sendConsoleMessage(strInfo);
            if (data[2] != null && display != null)
                display.status.setStatus(1, (String) data[2]);
          }
        }
        if (scriptEditor != null) {
          // general message has msWalltime = 0
          // special messages have msWalltime < 0
          // termination message has msWalltime > 0 (1 + msWalltime)
          // "script started"/"pending"/"script terminated"/"script completed"
          //   do not get sent to console
          if (msWalltime > 0) {
            // termination -- button legacy
            scriptEditor.notifyScriptTermination();
          } else if (msWalltime < 0) {
            if (msWalltime == -2)
              scriptEditor.notifyScriptStart();
          } else if (scriptEditor.isVisible() && ((String) data[2]).length() > 0 || data[4] != null) {
            System.out.println("Jmol notifyContext " + data[1]);
            scriptEditor.notifyContext((ScriptContext)viewer.getProperty("DATA_API", "scriptContext", null), data);
          }
        }
        break;
      case JmolConstants.CALLBACK_RESIZE:
      case JmolConstants.CALLBACK_SYNC:
      case JmolConstants.CALLBACK_HOVER:
      case JmolConstants.CALLBACK_MINIMIZATION:
        break;
      }
    }

    public void setCallbackFunction(String callbackType, String callbackFunction) {
      if (callbackType.equalsIgnoreCase("menu")) {
        menuStructure = callbackFunction;
        menuFile = null;
        //setupNewFrame(viewer.getStateInfo());
        return;
      }
      if (callbackType.equalsIgnoreCase("language")) {
//        new GT(callbackFunction);
//        language = GT.getLanguage();
//        Dialog.setupUIManager();
//        if (webExport != null) {
//          WebExport.saveHistory();
//          WebExport.dispose();
//          webExport = WebExport.createAndShowGUI(viewer, historyFile,
//              WEB_MAKER_WINDOW_NAME);
//        }
//        setupNewFrame(viewer.getStateInfo());
      }
    }

    /// end of JmolCallbackListener interface ///

    public String eval(String strEval) {
      if (strEval.startsWith("_GET_MENU"))
        return (jmolpopup == null ? "" : jmolpopup.getMenu("Jmol version "
            + Viewer.getJmolVersion() + "|" + strEval));
      sendConsoleMessage("javascript: " + strEval);
      return "# 'eval' is implemented only for the applet.";
    }

    /**
     *
     * @param fileName
     * @param type
     * @param text_or_bytes
     * @param quality
     * @return          null (canceled) or a message starting with OK or an error message
     */
    public String createImage(String fileName, String type, Object text_or_bytes,
                              int quality) {
      return createImageStatus(fileName, type, text_or_bytes, quality);
    }
  /**
   *
   * @param fileName
   * @param type
   * @param text_or_bytes
   * @param quality
   * @return          null (canceled) or a message starting with OK or an error message
   */
  String createImageStatus(String fileName, String type, Object text_or_bytes,
                               int quality)
  {
        String msg = (String) (new ImageCreator(viewer)).createImage(fileName,
            type, text_or_bytes, quality);
        if (msg == null || msg.startsWith("OK"))
          return msg;
        if (status != null) {
          status.setStatus(1, GT._("IO Exception:"));
          status.setStatus(2, msg);
        }
        return msg;
      }

    private void notifyAtomPicked(String info) {
      if (appConsole != null) {
        appConsole.sendConsoleMessage(info);
        appConsole.sendConsoleMessage("\n");
      }
    }

    private void notifyFileLoaded(String fullPathName, String fileName,
                                  String modelName, String errorMsg) {
      if (errorMsg != null) {
        return;
      }
      if (!jmolApp.haveDisplay)
        return;

      // this code presumes only ptLoad = -1 (error), 0 (zap), or 3 (completed)

      //      jmolpopup.updateComputedMenus();
      String title = "Jmol";
      if (fullPathName == null) {
        //if (fileName != null && appConsole != null)
          //appConsole.undoClear();
        // a 'clear/zap' operation
      } else {
        if (modelName != null && fileName != null)
          title = fileName + " - " + modelName;
        else if (fileName != null)
          title = fileName;
        else if (modelName != null)
          title = modelName;
        //recentFiles.notifyFileOpen(fullPathName);
      }
      frame.setTitle(title);
      if (atomSetChooser == null) {
        atomSetChooser = new AtomSetChooser(viewer, frame);
        pcs.addPropertyChangeListener(chemFileProperty, atomSetChooser);
      }
      pcs.firePropertyChange(chemFileProperty, null, null);
    }

    private void notifyFrameChanged(int frameNo, int file, int model) {
      // Note: twos-complement. To get actual frame number, use
      // Math.max(frameNo, -2 - frameNo)
      // -1 means all frames are now displayed
      boolean isAnimationRunning = (frameNo <= -2);

      /*
       * animationDirection is set solely by the "animation direction +1|-1" script command
       * currentDirection is set by operations such as "anim playrev" and coming to the end of
       * a sequence in "anim mode palindrome"
       *
       * It is the PRODUCT of these two numbers that determines what direction the animation is
       * going.
       *
       */
      //int animationDirection = (firstNo < 0 ? -1 : 1);
      //int currentDirection = (lastNo < 0 ? -1 : 1);
      //System.out.println("notifyFrameChange " + frameNo + " " + fileNo + " " + modelNo + " " + firstNo + " " + lastNo + " " + animationDirection + " " + currentDirection);
      if (display != null)
        display.status.setStatus(1, file + "." + model);
      if (jmolpopup == null || isAnimationRunning)
        return;
      jmolpopup.updateComputedMenus();
    }

    private void sendConsoleEcho(String strEcho) {
      if (appConsole != null)
        appConsole.sendConsoleEcho(strEcho);
    }

    private void sendConsoleMessage(String strStatus) {
      if (appConsole != null)
        appConsole.sendConsoleMessage(strStatus);
    }

    public void handlePopupMenu(int x, int y) {
      if (!language.equals(GT.getLanguage())) {
        jmolpopup = JmolPopup.newJmolPopup(viewer, true, menuStructure, true);
        language = GT.getLanguage();
      }
      jmolpopup.show(x, y);
    }

    public void showUrl(String url) {
      try {
        Class c = Class.forName("java.awt.Desktop");
        Method getDesktop = c.getMethod("getDesktop", new Class[] {});
        Object deskTop = getDesktop.invoke(null, new Class[] {});
        Method browse = c.getMethod("browse", new Class[] { URI.class });
        Object arguments[] = { new URI(url) };
        browse.invoke(deskTop, arguments);
      } catch (Exception e) {
        System.out.println(e.getMessage());
        if (appConsole != null) {
          appConsole
              .sendConsoleMessage("Java 6 Desktop.browse() capability unavailable. Could not open "
                  + url);
        } else {
          Logger
              .error("Java 6 Desktop.browse() capability unavailable. Could not open "
                  + url);
        }
      }
    }

    public void showConsole(boolean showConsole) {
      if (appConsole == null)
        return;
      if (showConsole)
        appConsole.setVisible(true);
      else
        appConsole.setVisible(false);
    }

    /**
     * this is just a test method for isosurface FUNCTIONXY
     * @param functionName
     * @param nX
     * @param nY
     * @return f(x,y) as a 2D array
     *
     */
    public float[][] functionXY(String functionName, int nX, int nY) {
      nX = Math.abs(nX);
      nY = Math.abs(nY);
      float[][] f = new float[nX][nY];
      //boolean isSecond = (functionName.indexOf("2") >= 0);
      for (int i = nX; --i >= 0;)
        for (int j = nY; --j >= 0;) {
          float x = i / 15f - 1;
          float y = j / 15f - 1;
          f[i][j] = (float) Math.sqrt(x*x + y);
          if (Float.isNaN(f[i][j]))
              f[i][j] = -(float) Math.sqrt(-x*x - y);
         // f[i][j] = (isSecond ? (float) ((i + j - nX) / (2f)) : (float) Math
           //   .sqrt(Math.abs(i * i + j * j)) / 2f);
          //if (i < 10 && j < 10)
          System.out.println(" functionXY " + i + " " + j + " " + f[i][j]);
        }

      return f; // for user-defined isosurface functions (testing only -- bob hanson)
    }

    public float[][][] functionXYZ(String functionName, int nX, int nY, int nZ) {
      nX = Math.abs(nX);
      nY = Math.abs(nY);
      nZ = Math.abs(nZ);
      float[][][] f = new float[nX][nY][nZ];
      for (int i = nX; --i >= 0;)
        for (int j = nY; --j >= 0;)
          for (int k = nZ; --k >= 0;) {
          float x = i / ((nX-1)/2f) - 1;
          float y = j / ((nY-1)/2f) - 1;
          float z = k / ((nZ-1)/2f) - 1;
          f[i][j][k] = (float) x*x + y - z*z;
          //if (i == 22 || i == 23)
            //System.out.println(" functionXYZ " + i + " " + j + " " + k + " " + f[i][j][k]);
        }
      return f; // for user-defined isosurface functions (testing only -- bob hanson)
    }

    public Hashtable getRegistryInfo() {
      return null;
    }

    public String dialogAsk(String type, String fileName) {
//      if (type.equals("load"))
//        return getOpenFileNameFromDialog(fileName);
//      if (type.equals("save")) {
//        return (new Dialog()).getSaveFileNameFromDialog(viewer, fileName,
//            null);
//      }
//      if (type.equals("saveImage")) {
//        Dialog sd = new Dialog();
//        fileName = sd.getImageFileNameFromDialog(viewer,
//            fileName, imageType, imageChoices, imageExtensions, qualityJPG,
//            qualityPNG);
//        imageType = sd.getType();
//        qualityJPG = sd.getQuality("JPG");
//        qualityPNG = sd.getQuality("PNG");
//        return fileName;
//      }
      return null;
    }
  }

  class ExecuteScriptAction extends AbstractAction {
    public ExecuteScriptAction() {
      super("executeScriptAction");
    }

    public void actionPerformed(ActionEvent e) {
      String script = e.getActionCommand();
      if (script.indexOf("#showMeasurementTable") >= 0)
        measurementTable.activate();
      //      viewer.script("set picking measure distance;set pickingstyle measure");
      viewer.evalStringQuiet(script);
    }
  }

}
