using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net;
using System.Text;
using System.IO;
using System.Web.Script.Serialization;


namespace TetraSlide.Login
{
    public class WebRequestUtility
    {
        public static T JsonHttpGet<T>(string url)
        {
            WebRequest request = HttpWebRequest.Create(url);
            request.Method = WebRequestMethods.Http.Get;
                        
            WebResponse response = request.GetResponse();
            using (Stream stream = response.GetResponseStream())
            {
                using (StreamReader reader = new StreamReader(stream))
                {
                    string jsonData = reader.ReadToEnd();

                    JavaScriptSerializer ser = new JavaScriptSerializer();

                    return ser.Deserialize<T>(jsonData);
                }
            }
        }

        public static T JsonHttpPost<T>(string url, string postData)
        {
            WebRequest request = HttpWebRequest.Create(url);
            request.Method = request.Method = WebRequestMethods.Http.Post;
            
            byte[] postBytes = Encoding.ASCII.GetBytes(postData);
            request.ContentType = "application/x-www-form-urlencoded";
            request.ContentLength = postBytes.Length;
            using (Stream requestStream = request.GetRequestStream())
            {
                requestStream.Write(postBytes, 0, postBytes.Length);
            }

            WebResponse response = request.GetResponse();
            using (Stream stream = response.GetResponseStream())
            {
                using (StreamReader reader = new StreamReader(stream))
                {
                    string jsonData = reader.ReadToEnd();

                    JavaScriptSerializer ser = new JavaScriptSerializer();
                    return ser.Deserialize<T>(jsonData);
                }
            }            
        }
    }
}