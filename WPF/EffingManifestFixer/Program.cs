using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace EffingManifestFixer
{

    


    class Program
    {
        static string ReadFile(string path)
        {
            return File.ReadAllText(path);
        }

        static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                System.Console.WriteLine("Usage: <program> <manifest file> <xml snippett file>");
                return;
            }

            XmlDocument manifest = new XmlDocument();
            manifest.LoadXml(ReadFile(args[0]));
            XmlDocumentFragment snippett = manifest.CreateDocumentFragment();
            snippett.InnerXml = ReadFile(args[1]);
            foreach (System.Xml.XmlNode element in snippett.SelectNodes("snippett/*"))
            {
                manifest.DocumentElement.AppendChild(element);
            }
            FileStream fstream = new FileStream(args[2], FileMode.Create);
            XmlWriterSettings xws = new XmlWriterSettings();
            xws.Indent = true;
            using (XmlWriter xmlWriter = XmlWriter.Create(fstream, xws))
            {
                manifest.WriteTo(xmlWriter);
            } 
        }
    }
}
