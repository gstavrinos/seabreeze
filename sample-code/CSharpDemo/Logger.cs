using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

public class Logger
{
    private TextBox textBox = null;

    public Logger() { }

    public Logger(TextBox tb) { textBox = tb; }

    public void setTextBox(TextBox tb) { textBox = tb; }

    // write (not append) log to textfile
    public void save(string pathname)
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
            }
        }
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
        string msg = String.Format(fmt, obj) + Environment.NewLine;
        Console.Write("LOG: {0}", msg);
    }
}