/*
		A basic implementation of the DeviceSetup class.
*/

import java.awt.*;

public class RFXPMSetupSetup extends DeviceSetup
{
	public RFXPMSetupSetup(Frame parent)
	{
		super(parent);
		
		// This code is automatically generated by Visual Cafe when you add
		// components to the visual environment. It instantiates and initializes
		// the components. To modify the code, only use code syntax that matches
		// what Visual Cafe can generate, or Visual Cafe may be unable to back
		// parse your Java file into its visual environment.
		//{{INIT_CONTROLS
		setDeviceProvider("150.178.3.101");
		setDeviceTitle("RFX PM Waveform setup");
		setDeviceType("RFXPMSetup");
		getContentPane().setLayout(null);
		setSize(623,441);
		deviceField1.setNumCols(20);
		deviceField1.setTextOnly(true);
		deviceField1.setOffsetNid(1);
		deviceField1.setLabelString("Comment: ");
		getContentPane().add(deviceField1);
		deviceField1.setBounds(12,12,360,36);
		deviceChoice1.setUpdateIdentifier("WAVE");
		deviceChoice1.setOffsetNid(2);
		{
			String[] tempString = new String[3];
			tempString[0] = "Voltage";
			tempString[1] = "Current";
			tempString[2] = "OpenLoop";
			deviceChoice1.setChoiceItems(tempString);
		}
		deviceChoice1.setLabelString("Control:");
		getContentPane().add(deviceChoice1);
		deviceChoice1.setBounds(12,96,180,40);
		deviceWave1.setIdentifier("pmwave");
		deviceWave1.setUpdateExpression("AAMaxValue(_window, $, _config)");
		deviceWave1.setUpdateIdentifier("WAVE");
		deviceWave1.setOffsetNid(3);
		getContentPane().add(deviceWave1);
		deviceWave1.setBounds(12,156,564,240);
		{
			String[] tempString = new String[1];
			tempString[0] = "Total magnetizing current above limit";
			deviceButtons1.setCheckMessages(tempString);
		}
		{
			String[] tempString = new String[1];
			tempString[0] = "maxval(_pmwave)*\\pconfig:pm_enabled <= \\pconfig:max_i_mag";
			deviceButtons1.setCheckExpressions(tempString);
		}
		getContentPane().add(deviceButtons1);
		deviceButtons1.setBounds(168,396,281,36);
		deviceField2.setNumCols(3);
		deviceField2.setIdentifier("window");
		deviceField2.setOffsetNid(8);
		deviceField2.setLabelString(" Window: ");
		deviceField2.setDisplayEvaluated(true);
		deviceField2.setEditable(false);
		getContentPane().add(deviceField2);
		deviceField2.setBounds(252,48,120,40);
		deviceField4.setTextOnly(true);
		deviceField3.setBounds(192,96,204,40);
		deviceField7.setOffsetNid(11);
		deviceField7.setLabelString("PTCB Voltage (V):");
		getContentPane().add(deviceField7);
		deviceField7.setBounds(372,48,240,40);
		deviceField4.setOffsetNid(10);
		deviceField4.setLabelString("Enabled units: ");
		deviceField4.setDisplayEvaluated(true);
		deviceField4.setEditable(false);
		getContentPane().add(deviceField4);
		deviceField4.setBounds(0,48,252,40);
		deviceField5.setNumCols(8);
		deviceField5.setOffsetNid(13);
		deviceField5.setLabelString("Tot. Current (A): ");
		deviceField5.setDisplayEvaluated(true);
		deviceField5.setEditable(false);
		getContentPane().add(deviceField5);
		deviceField5.setBounds(396,96,216,40);
		deviceField6.setNumCols(8);
		deviceField6.setOffsetNid(12);
		deviceField6.setLabelString("Transfer R (Ohm):");
		getContentPane().add(deviceField6);
		deviceField6.setBounds(372,12,240,40);
		deviceField3.setIdentifier("config");
		deviceField3.setTextOnly(true);
		deviceField3.setOffsetNid(9);
		deviceField3.setLabelString("Config.:");
		deviceField3.setDisplayEvaluated(true);
		deviceField3.setEditable(false);
		getContentPane().add(deviceField3);
		//}}
	}

    public RFXPMSetupSetup()
	{
		this((Frame)null);
	}

	public RFXPMSetupSetup(String sTitle)
	{
		this();
		setTitle(sTitle);
	}

	static public void main(String args[])
	{
		(new RFXPMSetupSetup()).setVisible(true);
	}

	public void addNotify()
	{
		// Record the size of the window prior to calling parents addNotify.
		Dimension size = getSize();

		super.addNotify();

		if (frameSizeAdjusted)
			return;
		frameSizeAdjusted = true;

		// Adjust size of frame according to the insets
		Insets insets = getInsets();
		setSize(insets.left + insets.right + size.width, insets.top + insets.bottom + size.height);
	}

	// Used by addNotify
	boolean frameSizeAdjusted = false;

	//{{DECLARE_CONTROLS
	DeviceField deviceField1 = new DeviceField();
	DeviceChoice deviceChoice1 = new DeviceChoice();
	DeviceWave deviceWave1 = new DeviceWave();
	DeviceButtons deviceButtons1 = new DeviceButtons();
	DeviceField deviceField2 = new DeviceField();
	DeviceField deviceField4 = new DeviceField();
	DeviceField deviceField5 = new DeviceField();
	DeviceField deviceField6 = new DeviceField();
	DeviceField deviceField3 = new DeviceField();
	DeviceField deviceField7 = new DeviceField();
	//}}

}