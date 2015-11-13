using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

public class Logger
{
    private bool verbose = false;
    private List<string> buffer = new List<string>();
    private TextBox textBox = null;
    private static Logger instance = null;

    public static Logger getInstance()
    {
        if (instance == null)
            instance = new Logger();
        return instance;
    }

    private Logger() { }

    public void setTextBox(TextBox tb) { textBox = tb; }

    public void setVerbose(bool flag)
    {
        verbose = flag;
    }

    // write (not append) log to textfile
    public bool save(string pathname)
    {
        if (textBox != null)
        {
            try
            {
                TextWriter tw = new StreamWriter(pathname);
                tw.WriteLine(textBox.Text);
                tw.Close();
            }
            catch (Exception e)
            {
                display("Error saving log to {0}: {1}", pathname, e.Message);
                return false;
            }
        }
        return true;
    }

    // log to textbox and console
    public void display(string fmt, params Object[] obj)
    {
        string msg = String.Format(fmt, obj) +Environment.NewLine;
        Console.Write("DISPLAY: {0}", msg);
        if (textBox != null)
            textBox.AppendText(msg);
    }

    // just log to console
    public void log(string fmt, params Object[] obj)
    {
        if (verbose)
        {
            display(fmt, obj);
        }
        else
        {
            string msg = String.Format(fmt, obj) + Environment.NewLine;
            Console.Write("LOG: {0}", msg);
        }
    }

    public void flush()
    {
        foreach (string s in buffer)
            display(s);
        buffer.Clear();
    }

    public void queue(string fmt, params Object[] obj)
    {
        string msg = String.Format(fmt, obj);
        buffer.Add(msg);
    }
}
