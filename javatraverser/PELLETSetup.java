/*
 * PELLETSetup.java
 *
 * Created on March 26, 2007, 4:49 PM
 */

/**
 *
 * @author  taliercio
 */
public class PELLETSetup extends DeviceSetup {
    
    /** Creates new form PELLETSetup */
    public PELLETSetup() {
        initComponents();
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc=" Generated Code ">//GEN-BEGIN:initComponents
    private void initComponents() {
        jCheckBox1 = new javax.swing.JCheckBox();
        deviceButtons1 = new DeviceButtons();
        jPanel1 = new javax.swing.JPanel();
        jPanel2 = new javax.swing.JPanel();
        deviceField1 = new DeviceField();
        deviceDispatch1 = new DeviceDispatch();
        jPanel3 = new javax.swing.JPanel();
        deviceField2 = new DeviceField();
        deviceChoice1 = new DeviceChoice();
        jPanel8 = new javax.swing.JPanel();
        jPanel9 = new javax.swing.JPanel();
        deviceChannel1 = new DeviceChannel();
        deviceField11 = new DeviceField();
        jPanel10 = new javax.swing.JPanel();
        deviceChannel2 = new DeviceChannel();
        deviceField12 = new DeviceField();
        jPanel11 = new javax.swing.JPanel();
        jPanel12 = new javax.swing.JPanel();
        deviceChannel3 = new DeviceChannel();
        deviceField13 = new DeviceField();
        jPanel13 = new javax.swing.JPanel();
        deviceChannel4 = new DeviceChannel();
        deviceField14 = new DeviceField();
        jPanel14 = new javax.swing.JPanel();
        jPanel15 = new javax.swing.JPanel();
        deviceChannel5 = new DeviceChannel();
        deviceField15 = new DeviceField();
        jPanel16 = new javax.swing.JPanel();
        deviceChannel6 = new DeviceChannel();
        deviceField16 = new DeviceField();
        jPanel17 = new javax.swing.JPanel();
        jPanel18 = new javax.swing.JPanel();
        deviceChannel7 = new DeviceChannel();
        deviceField17 = new DeviceField();
        jPanel19 = new javax.swing.JPanel();
        deviceChannel8 = new DeviceChannel();
        deviceField18 = new DeviceField();

        jCheckBox1.setText("jCheckBox1");
        jCheckBox1.setBorder(javax.swing.BorderFactory.createEmptyBorder(0, 0, 0, 0));
        jCheckBox1.setMargin(new java.awt.Insets(0, 0, 0, 0));

        setDeviceProvider("150.178.32.79");
        setDeviceTitle("Pellet Setup");
        setDeviceType("PELLET");
        setHeight(300);
        setWidth(550);
        deviceButtons1.setCheckExpressions(new String[] {});
        deviceButtons1.setCheckMessages(new String[] {});
        deviceButtons1.setMethods(new String[] {"init", "store"});
        getContentPane().add(deviceButtons1, java.awt.BorderLayout.SOUTH);

        jPanel1.setLayout(new java.awt.GridLayout(6, 0));

        jPanel2.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT));

        deviceField1.setIdentifier("");
        deviceField1.setLabelString("Comment:");
        deviceField1.setNumCols(30);
        deviceField1.setOffsetNid(2);
        deviceField1.setTextOnly(true);
        jPanel2.add(deviceField1);

        jPanel2.add(deviceDispatch1);

        jPanel1.add(jPanel2);

        jPanel3.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT));

        deviceField2.setIdentifier("");
        deviceField2.setLabelString("RS232:");
        deviceField2.setNumCols(25);
        deviceField2.setOffsetNid(1);
        jPanel3.add(deviceField2);

        deviceChoice1.setChoiceItems(new String[] {"ENABLED", "DISABLED"});
        deviceChoice1.setIdentifier("");
        deviceChoice1.setLabelString("Pellet State : ");
        deviceChoice1.setOffsetNid(144);
        deviceChoice1.setUpdateIdentifier("");
        jPanel3.add(deviceChoice1);

        jPanel1.add(jPanel3);

        jPanel8.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT, 5, 0));

        deviceChannel1.setInSameLine(true);
        deviceChannel1.setLabelString("P1 (Medium)");
        deviceChannel1.setOffsetNid(10);
        deviceChannel1.setShowVal("");
        deviceChannel1.setUpdateIdentifier("");
        jPanel9.add(deviceChannel1);

        deviceField11.setIdentifier("");
        deviceField11.setOffsetNid(11);
        jPanel9.add(deviceField11);

        jPanel8.add(jPanel9);

        deviceChannel2.setInSameLine(true);
        deviceChannel2.setLabelString("P5 (Big)");
        deviceChannel2.setOffsetNid(74);
        deviceChannel2.setShowVal("");
        deviceChannel2.setUpdateIdentifier("");
        jPanel10.add(deviceChannel2);

        deviceField12.setIdentifier("");
        deviceField12.setOffsetNid(75);
        jPanel10.add(deviceField12);

        jPanel8.add(jPanel10);

        jPanel1.add(jPanel8);

        jPanel11.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT, 5, 0));

        deviceChannel3.setInSameLine(true);
        deviceChannel3.setLabelString("P2 (Small)");
        deviceChannel3.setOffsetNid(26);
        deviceChannel3.setShowVal("");
        deviceChannel3.setUpdateIdentifier("");
        jPanel12.add(deviceChannel3);

        deviceField13.setIdentifier("");
        deviceField13.setOffsetNid(27);
        jPanel12.add(deviceField13);

        jPanel11.add(jPanel12);

        deviceChannel4.setInSameLine(true);
        deviceChannel4.setLabelString("P6 (Small)");
        deviceChannel4.setOffsetNid(90);
        deviceChannel4.setShowVal("");
        deviceChannel4.setUpdateIdentifier("");
        jPanel13.add(deviceChannel4);

        deviceField14.setIdentifier("");
        deviceField14.setOffsetNid(91);
        jPanel13.add(deviceField14);

        jPanel11.add(jPanel13);

        jPanel1.add(jPanel11);

        jPanel14.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT, 5, 0));

        deviceChannel5.setInSameLine(true);
        deviceChannel5.setLabelString("P3 (Big)");
        deviceChannel5.setOffsetNid(42);
        deviceChannel5.setShowVal("");
        deviceChannel5.setUpdateIdentifier("");
        jPanel15.add(deviceChannel5);

        deviceField15.setIdentifier("");
        deviceField15.setOffsetNid(43);
        jPanel15.add(deviceField15);

        jPanel14.add(jPanel15);

        deviceChannel6.setInSameLine(true);
        deviceChannel6.setLabelString("P7 (Medium)");
        deviceChannel6.setOffsetNid(106);
        deviceChannel6.setShowVal("");
        deviceChannel6.setUpdateIdentifier("");
        jPanel16.add(deviceChannel6);

        deviceField16.setIdentifier("");
        deviceField16.setOffsetNid(107);
        jPanel16.add(deviceField16);

        jPanel14.add(jPanel16);

        jPanel1.add(jPanel14);

        jPanel17.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT, 5, 0));

        deviceChannel7.setInSameLine(true);
        deviceChannel7.setLabelString("P4 (Small)");
        deviceChannel7.setOffsetNid(58);
        deviceChannel7.setShowVal("");
        deviceChannel7.setUpdateIdentifier("");
        jPanel18.add(deviceChannel7);

        deviceField17.setIdentifier("");
        deviceField17.setOffsetNid(59);
        jPanel18.add(deviceField17);

        jPanel17.add(jPanel18);

        deviceChannel8.setInSameLine(true);
        deviceChannel8.setLabelString("P8 (Small)");
        deviceChannel8.setOffsetNid(122);
        deviceChannel8.setShowVal("");
        deviceChannel8.setUpdateIdentifier("");
        jPanel19.add(deviceChannel8);

        deviceField18.setIdentifier("");
        deviceField18.setOffsetNid(123);
        jPanel19.add(deviceField18);

        jPanel17.add(jPanel19);

        jPanel1.add(jPanel17);

        getContentPane().add(jPanel1, java.awt.BorderLayout.CENTER);

    }// </editor-fold>//GEN-END:initComponents
    
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private DeviceButtons deviceButtons1;
    private DeviceChannel deviceChannel1;
    private DeviceChannel deviceChannel2;
    private DeviceChannel deviceChannel3;
    private DeviceChannel deviceChannel4;
    private DeviceChannel deviceChannel5;
    private DeviceChannel deviceChannel6;
    private DeviceChannel deviceChannel7;
    private DeviceChannel deviceChannel8;
    private DeviceChoice deviceChoice1;
    private DeviceDispatch deviceDispatch1;
    private DeviceField deviceField1;
    private DeviceField deviceField11;
    private DeviceField deviceField12;
    private DeviceField deviceField13;
    private DeviceField deviceField14;
    private DeviceField deviceField15;
    private DeviceField deviceField16;
    private DeviceField deviceField17;
    private DeviceField deviceField18;
    private DeviceField deviceField2;
    private javax.swing.JCheckBox jCheckBox1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel10;
    private javax.swing.JPanel jPanel11;
    private javax.swing.JPanel jPanel12;
    private javax.swing.JPanel jPanel13;
    private javax.swing.JPanel jPanel14;
    private javax.swing.JPanel jPanel15;
    private javax.swing.JPanel jPanel16;
    private javax.swing.JPanel jPanel17;
    private javax.swing.JPanel jPanel18;
    private javax.swing.JPanel jPanel19;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel8;
    private javax.swing.JPanel jPanel9;
    // End of variables declaration//GEN-END:variables


static public void main(String args[])
{
    PELLETSetup ps = new PELLETSetup();
    ps.pack();
    ps.setVisible(true);
    
}

}

