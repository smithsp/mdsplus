//package jTraverser;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class DataEditor
    extends JPanel
    implements ActionListener, Editor
{
  ExprEditor expr_edit;
  LabeledExprEditor units_edit;
  ParameterEditor param_edit;
  PythonEditor python_edit;
  JComboBox combo;
  JPanel mode_panel;
  int mode_idx, curr_mode_idx;
  Data data;
  Data units;
  boolean editable = true;
  TreeDialog dialog;

  public DataEditor(Data data, TreeDialog dialog)
  {
    this.dialog = dialog;
    this.data = data;
    if (data == null)
    {
      mode_idx = 0;
      this.data = null;
      units = null;
    }
    else
    {
      if (data instanceof ParameterData)
          mode_idx = 2;
      else if(data instanceof FunctionData && ((FunctionData)data).opcode == PythonEditor.OPC_FUN)
      {
          Data[] args = ((FunctionData)data).getArgs();
          try {
          if(args != null && args.length > 2 && args[1] != null && (args[1] instanceof StringData) &&
            args[1].getString()!= null && args[1].getString().toUpperCase().equals("PY"))
                mode_idx = 3;
            else
                mode_idx = 1;
          }catch(Exception exc){mode_idx = 1;}
      } 
      else
          mode_idx = 1;
      if (data.dtype == Data.DTYPE_WITH_UNITS)
      {
        this.data = ( (WithUnitsData) data).getDatum();
        units = ( (WithUnitsData) data).getUnits();
      }
      else
      {
        this.data = data;
        units = null;
      }
    }

    curr_mode_idx = mode_idx;
    String names[] =
        {
        "Undefined", "Expression", "Parameter", "Python Expression"};
    combo = new JComboBox(names);
    combo.setEditable(false);
    combo.setSelectedIndex(mode_idx);
    combo.addActionListener(this);
    mode_panel = new JPanel();
    mode_panel.add(combo);
    setLayout(new BorderLayout());
    add(mode_panel, "North");
    addEditor();
  }

  private void addEditor()
  {
    switch (curr_mode_idx)
    {
      case 0:
        return;
      case 1:
        if (data != null && data.dtype == Data.DTYPE_T)
          expr_edit = new ExprEditor(data, true, 8, 30);
        else
          expr_edit = new ExprEditor(data, false, 8, 30);
        units_edit = new LabeledExprEditor("Units: ", new ExprEditor(units, true));
        add(expr_edit, "Center");
        mode_panel.add(units_edit);
        break;
      case 2:
        if (data != null && data instanceof ParameterData)
        {
          param_edit = new ParameterEditor(new ExprEditor( ( (ParameterData)data).getDatum(), false, 1, 30),
                                           new ExprEditor( ( (ParameterData)data).getHelp(), true, 6, 10),
                                           new ExprEditor( ( (ParameterData)data).getValidation(), false, 1, 30));
        }
        else
        {
          param_edit = new ParameterEditor(new ExprEditor(data, false, 1, 30),
                                           new ExprEditor(null, true, 8, 10),
                                           new ExprEditor(null, false, 1, 30));
        }
        units_edit = new LabeledExprEditor("Units: ", new ExprEditor(units, true));
        add(param_edit, "Center");
        mode_panel.add(units_edit);
        break;
      case 3: 
        if (data != null && data instanceof FunctionData)
        {
          python_edit = new PythonEditor(((FunctionData)data).getArgs());
        }
        else
        {
          python_edit = new PythonEditor(null);
        }
         add(python_edit, "Center");
         units_edit = new LabeledExprEditor("Units: ", new ExprEditor(units, true));
         mode_panel.add(units_edit);
         break;
          
    }
  }

  public void actionPerformed(ActionEvent e)
  {
    if (!editable)
    {
      combo.setSelectedIndex(curr_mode_idx);
      return;
    }
    int idx = combo.getSelectedIndex();
    if (idx == curr_mode_idx)
      return;
    if (curr_mode_idx == 1)
    {
      remove(expr_edit);
      mode_panel.remove(units_edit);
    }
    if (curr_mode_idx == 2)
    {
      remove(param_edit);
      mode_panel.remove(units_edit);
    }
    if (curr_mode_idx == 3)
    {
      remove(python_edit);
      mode_panel.remove(units_edit);
    }
    curr_mode_idx = idx;
    addEditor();
    validate();
    dialog.repack();
  }

  public void reset()
  {
    if (curr_mode_idx == 1)
    {
      remove(expr_edit);
      mode_panel.remove(units_edit);
    }
    if (curr_mode_idx == 2)
    {
      remove(param_edit);
      mode_panel.remove(units_edit);
    }
    if (curr_mode_idx == 3)
    {
      remove(python_edit);
      mode_panel.remove(units_edit);
    }
    curr_mode_idx = mode_idx;
    combo.setSelectedIndex(mode_idx);
    addEditor();
    validate();
    dialog.repack();
  }

  public Data getData()
  {
    Data units;
    switch (curr_mode_idx)
    {
      case 0:
        return null;
      case 1:
        units = units_edit.getData();
        if (units != null)
        {
          if (units instanceof StringData &&
              ( (StringData) units).datum.equals(""))
            return expr_edit.getData();
          else
            return new WithUnitsData(expr_edit.getData(), units);
        }
        else
          return expr_edit.getData();
        case 2:
          units = units_edit.getData();
          if (units != null)
          {
            if (units instanceof StringData &&
                ( (StringData) units).datum.equals(""))
              return param_edit.getData();
            else
              return new WithUnitsData(param_edit.getData(), units);
          }
          else
            return param_edit.getData();

        case 3:
          units = units_edit.getData();
          if (units != null)
          {
            if (units instanceof StringData &&
                ( (StringData) units).datum.equals(""))
              return python_edit.getData();
            else
              return new WithUnitsData(python_edit.getData(), units);
          }
          else
            return python_edit.getData();

    }
    return null;
  }

  public void setData(Data data)
  {
    this.data = data;
    if (data == null)
    {
      mode_idx = 0;
      this.data = null;
      units = null;
    }
    else
    {
      if(data instanceof ParameterData)
        mode_idx = 2;
      else if(data instanceof FunctionData && ((FunctionData)data).opcode == PythonEditor.OPC_FUN)
      {
          Data[] args = ((FunctionData)data).getArgs();
          try {
          if(args != null && args.length > 2 && args[1] != null && (args[1] instanceof StringData) &&
            args[1].getString()!= null && args[1].getString().toUpperCase().equals("PY"))
                mode_idx = 3;
            else
                mode_idx = 1;
          }catch(Exception exc){mode_idx = 1;}
      } 
      else
        mode_idx = 1;
      if (data.dtype == Data.DTYPE_WITH_UNITS)
      {
        this.data = ( (WithUnitsData) data).getDatum();
        units = ( (WithUnitsData) data).getUnits();
      }
      else
      {
        this.data = data;
        units = null;
      }
    }
    reset();
  }

  public void setEditable(boolean editable)
  {
    this.editable = editable;
    if (expr_edit != null)
      expr_edit.setEditable(editable);
    if (python_edit != null)
      python_edit.setEditable(editable);
    if (units_edit != null)
      units_edit.setEditable(editable);
  }

}