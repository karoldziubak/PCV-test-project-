using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestownikKolorow
{
    public partial class Form1 : Form
    {
        String exePath = Application.StartupPath; //interface path
        public Form1()
        {
            InitializeComponent();
            //process1.StartInfo.WorkingDirectory = exePath + "\\app\\bin\\Debug";
            //process1.StartInfo.FileName = exePath + "\\app\\bin\\Debug\\SFMLtemp.exe";
            process1.StartInfo.WorkingDirectory = exePath + "\\bin";
            process1.StartInfo.FileName = exePath + "\\bin\\SFMLtemp.exe";

        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            //Order:
            //[0]-idk
            //[1]-Circles number
            //[2]-Blur level
            //[3]-Name
            //[4]-Color range (1- full, 0-red)
            //[5]-Edge type(1-Gauss, 0- linear)
            //[6]-Point mode(1-pulse,2-rotate+pulse,3-rotate)
            //[7]-Face distance
            int mode = 0;
            if (radioButton6.Checked)
            {
                mode = 2;
            }else if (radioButton5.Checked)
            {
                mode = 1;
            }else if (radioButton7.Checked)
            {
                mode = 3;
            }
            process1.StartInfo.Arguments = Convert.ToString(numericUpDown1.Value)
                +" "+Convert.ToString(numericUpDown2.Value)
                +" "+Convert.ToString(textBox1.Text)
                +" "+Convert.ToString(Convert.ToInt64(radioButton1.Checked))
                +" "+Convert.ToString(Convert.ToInt64(radioButton3.Checked))
                +" "+Convert.ToString(mode)
                +" "+Convert.ToString(numericUpDown3.Value)
                +" "+Convert.ToString(numericUpDown4.Value);
            process1.StartInfo.Arguments = process1.StartInfo.Arguments.Replace(",", ".");
            process1.Start();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioButton7_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
