package jScope;

/* $Id$ */
import jScope.WaveformEvent;
import jScope.WaveInterface;
import jScope.Waveform;
import jScope.UpdateEvent;
import jScope.UpdateEventListener;
import jScope.TwuNameServices;
import jScope.Signal;
import jScope.MultiWaveform;
import jScope.DataProvider;
import jScope.FrameData;
import jScope.MdsWaveInterface;
import jScope.ColorMap;
import java.awt.*;
import java.awt.datatransfer.DataFlavor;
import java.util.*;
import java.io.*;
import javax.swing.SwingUtilities;
import java.awt.image.*;
import java.awt.geom.*;
import javax.swing.TransferHandler;

/**
 Class MultiWaveform extends the capability of class Waveform to deal with multiple
 waveforms.
 */
public class jScopeMultiWave
    extends MultiWaveform
    implements UpdateEventListener
{
    String eventName;
    public jScopeMultiWave(DataProvider dp, jScopeDefaultValues def_values,
                           boolean cache_enabled)
    {
        super();
        wi = new MdsWaveInterface(this, dp, def_values, cache_enabled);
        setTransferHandler(new ToTransferHandler());
    }

    public void processUpdateEvent(UpdateEvent e)
    {
        eventName = e.name;
        SwingUtilities.invokeLater(new Runnable()
        {
            public void run()
            {
                //System.out.println("Evento su waveform "+e.name);
                WaveformEvent we = new WaveformEvent(jScopeMultiWave.this,
                    WaveformEvent.EVENT_UPDATE, "Update on event " + eventName);
                dispatchWaveformEvent(we);
            }
        });
    }


    public void RefreshOnEvent()
    {
        /*
        setCursor(new Cursor(Cursor.WAIT_CURSOR));
        try
        {
            AddEvent();
        }
        catch (IOException e)
        {}
        */

       /*
        Thread p = new Thread()
        {
            public void run()
            {
       */
                MdsWaveInterface mwi = (MdsWaveInterface) wi;
                boolean cache_state = mwi.cache_enabled;
                mwi.cache_enabled = false;
                try {
                    mwi.refresh();
                }catch(Exception exc)
                {
                    System.out.println(exc);
                }
                mwi.cache_enabled = cache_state;

                SwingUtilities.invokeLater(new Runnable()
                {
                    public void run()
                    {
                        jScopeWaveUpdate();
                    }
                });
        /*
            }
        };
        p.start();
        */
    }


    public void Refresh()
    {
        setCursor(new Cursor(Cursor.WAIT_CURSOR));
        try
        {
            AddEvent();
        }
        catch (IOException e)
        {}

        Thread p = new Thread()
        {
            public void run()
            {
                MdsWaveInterface mwi = (MdsWaveInterface) wi;
                boolean cache_state = mwi.cache_enabled;
                try {
                    mwi.refresh();
                }catch(Exception exc){}

                SwingUtilities.invokeLater(new Runnable()
                    {
                        public void run()
                        {
                            jScopeWaveUpdate();
                        }
                    });
            }
        };
        p.start();
    }

    public void setColorMap(ColorMap colorMap)
    {
        super.setColorMap(colorMap);
        wi.setColorMap(colorMap);
    }

    public ColorMap getColorMap()
    {
        return wi.getColorMap();
    }

    public void jScopeErase()
    {
        Erase();
        wi.Erase();
    }

    public String getBriefError(String er, boolean brief)
    {
        if (brief)
        {
            int idx = (er.indexOf('\n') == -1 ? er.length() : er.indexOf('\n'));
            er = er.substring(0, idx);
        }
        return er;
    }

    public synchronized void jScopeWaveUpdate()
    {
        String out_error;

        if (wi.isAddSignal())
        {
            //reset to previous configuration if signal/s are not added
            if ( ( (MdsWaveInterface) wi).prev_wi != null &&
                ( (MdsWaveInterface) wi).prev_wi.GetNumEvaluatedSignal() ==
                ( (MdsWaveInterface) wi).GetNumEvaluatedSignal())
            {
                ( (MdsWaveInterface) wi).prev_wi.error = ( (MdsWaveInterface)
                    wi).error;
                ( (MdsWaveInterface) wi).prev_wi.w_error = ( (MdsWaveInterface)
                    wi).w_error;
                ( (MdsWaveInterface) wi).prev_wi.setAddSignal(wi.isAddSignal());
                wi = ( (MdsWaveInterface) wi).prev_wi;
                wi.SetIsSignalAdded(false);
            }
            else
                wi.SetIsSignalAdded(true);

            ( (MdsWaveInterface) wi).prev_wi = null;
        }
        Update(wi);
        WaveformEvent e = new WaveformEvent(this, WaveformEvent.END_UPDATE);
        dispatchWaveformEvent(e);
    }

    public void Update(WaveInterface _wi)
    {
        wi = (MdsWaveInterface) _wi;

        resetMode();

        orig_signals = null;
        super.x_label = wi.xlabel;
        super.y_label = wi.ylabel;
        super.z_label = wi.zlabel;
        super.x_log = wi.x_log;
        super.y_log = wi.y_log;

        //  String error = null;
        //  if(!wi.isAddSignal())
        wave_error = wi.getErrorTitle(true);

        if (wi.title != null)
            super.title = wi.title;
        else
            super.title = "";

        setColorMap(wi.getColorMap());

        super.show_legend = wi.show_legend;
        super.legend_x = wi.legend_x;
        super.legend_y = wi.legend_y;
        super.is_image = wi.is_image;
        SetFrames(wi.getFrames());

        if (wi.signals != null)
        {
            boolean all_null = true;
            for (int i = 0; i < wi.signals.length; i++)
                if (wi.signals[i] != null)
                {
                    all_null = false;
                    if (wi.in_label[i] != null && wi.in_label[i].length() != 0)
                        wi.signals[i].setName(wi.in_label[i]);
                    else
                        wi.signals[i].setName(wi.in_y[i]);
                    wi.signals[i].setMarker(wi.markers[i]);
                    wi.signals[i].setMarkerStep(wi.markers_step[i]);
                    wi.signals[i].setInterpolate(wi.interpolates[i]);
                    wi.signals[i].setColorIdx(wi.colors_idx[i]);

                    wi.signals[i].setMode1D( (int) wi.mode1D[i]);
                    wi.signals[i].setMode2D( (int) wi.mode2D[i]);


                }
            if (!all_null)
            {
                Update(wi.signals);
                return;
            }
        }

        if (wi.is_image && wi.getFrames() != null)
        {
            super.frames.setAspectRatio(wi.keep_ratio);
            super.curr_point_sig_idx = 0;

            if (signals.size() != 0)
                signals.removeAllElements();

            if( wi.getModified() )
                frame = 0;
            
            not_drawn = true;
            super.Update();
            return;
        }
        Erase();
    }

    protected Color getSignalColor(int i)
    {
        if (i > wi.num_waves)
            return Color.black;
        return colors[wi.colors_idx[i] % Waveform.colors.length];
    }

    public int GetMarker(int idx)
    {
        if (idx < wi.num_waves)
        {
            return wi.markers[idx];
        }
        return 0;
    }

    public int getSignalCount()
    {
        return wi.num_waves;
    }

    public String[] GetSignalsName()
    {
        return wi.GetSignalsName();
    }

    public boolean[] GetSignalsState()
    {
        return wi.GetSignalsState();
    }

    public void SetSignalState(String label, boolean state)
    {
        Signal sig;
        wi.setSignalState(label, state);
        super.SetSignalState(label, state);
    }

    protected String getSignalInfo(int i)
    {
        String s;
        String name = (wi.in_label != null && wi.in_label[i] != null &&
                       wi.in_label[i].length() > 0) ? wi.in_label[i] :
            wi.in_y[i];
        String er = (wi.w_error != null && wi.w_error[i] != null) ? " ERROR " :
            "";
        if (wi.shots != null)
        {
            s = name + " " + wi.shots[i] + er;
        }
        else
        {
            s = name + er;
        }

        if (signals.size() > i)
        {
            Signal sign = (Signal) signals.elementAt(i);
            if (sign != null && sign.getType() == Signal.TYPE_2D)
            {
                switch (sign.getMode2D())
                {
                    case Signal.MODE_XZ:
                        s = s + " [X-Z Y = " +
                            Waveform.ConvertToString(sign.getYinXZplot(), false) +
                            " ]";
                        break;
                    case Signal.MODE_YZ:
                        s = s + " [Y-Z X = " + sign.getStringOfXinYZplot() +
                             //Waveform.ConvertToString(sign.getTime(), false) +
                            " ]";
                        break;
                        /*
                    case Signal.MODE_YX:
                        s = s + " [Y-X T = " +  sign.getStringTime() +
                            //Waveform.ConvertToString(sign.getTime(), false) +
                            " ]";
                        break;
                        */
                }
            }
        }

        // TWU Signal URLs
        // If the signal is a TWU URL, we would like it to be displayed as a URL.
        // I hope that this does not clash with other jScope codes.  If so, tell me!
        // J.G.Krom (Textor, Juelich, Germany) <J.Krom@fz-juelich.de>

        if (TwuNameServices.catersFor(wi.dp))
            s = TwuNameServices.legendString(wi, name,
                                             wi.shots == null ? 0 : wi.shots[i]);

        return s;
    }

    protected boolean isSignalShow(int i)
    {
        return wi.GetSignalState(i);
    }

    public void AddEvent(String event) throws IOException
    {
        ( (MdsWaveInterface) wi).AddEvent(this, event);
    }

    public void RemoveEvent(String event) throws IOException
    {
        ( (MdsWaveInterface) wi).AddEvent(this, event);
    }

    public void AddEvent() throws IOException
    {
        ( (MdsWaveInterface) wi).AddEvent(this);
    }

    public void RemoveEvent() throws IOException
    {
        ( (MdsWaveInterface) wi).RemoveEvent(this);
    }

    public void removeNotify()
    {
        //System.out.println("Rimuovo jScopeMultiWave");
        try
        {
            RemoveEvent();
        }
        catch (IOException e)
        {}

        this.wi = null;
        signals = null;
        orig_signals = null;
        Graphics g = getGraphics();
        g.dispose();
        super.removeNotify();
    }

    protected void DrawImage(Graphics g, Object img, Dimension dim, int type)
    {
        if ( type != FrameData.JAI_IMAGE )
        {
            super.DrawImage(g, img, dim, type);
        }
        else
        {
            ( (Graphics2D) g).clearRect(0, 0, dim.width, dim.height);
            ( (Graphics2D) g).drawRenderedImage( (RenderedImage) img,
                                                new
                                                AffineTransform(1f, 0f, 0f, 1f,
                0F, 0F));
        }
    }
    
          //Inner class ToTransferHandler to receive jTraverser info
    class ToTransferHandler extends TransferHandler
    {
        public boolean canImport(TransferHandler.TransferSupport support)
        {
            if(!support.isDrop())
                return false;
            if(!support.isDataFlavorSupported(DataFlavor.stringFlavor))
                return false;
            if((support.getSourceDropActions() & TransferHandler.COPY_OR_MOVE) == 0)
                return false;
            //support.setDropAction(TransferHandler.COPY);
            return true;
        }
        public boolean importData(TransferHandler.TransferSupport support)
        {
            if(!canImport(support))
                return false;
            try {
                
                String data = (String)support.getTransferable().getTransferData(DataFlavor.stringFlavor);
               StringTokenizer st = new StringTokenizer(data, ":");
                String experiment = st.nextToken();
                String path = data.substring(experiment.length()+1);
                 if(support.getDropAction() ==  TransferHandler.MOVE)
                    wi.Erase();
                wi.setExperiment(experiment);
                wi.AddSignal(path);
                WaveformEvent we = new WaveformEvent(jScopeMultiWave.this,
                    WaveformEvent.EVENT_UPDATE, "Update on Drop event ");
                dispatchWaveformEvent(we);
            }catch(Exception exc)
            {
                return false;
            }
            return true;
        }
     }
    


}