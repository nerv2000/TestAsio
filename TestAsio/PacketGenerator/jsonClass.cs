using System.Collections.Generic;
using Newtonsoft.Json;

namespace PacketGenerator.JsonClass
{
    public class Parameter
    {
        public string Type { get; set; }
        public string Name { get; set; }
    }

    public class Function
    {
        public string Name { get; set; }
        public List<Parameter> Parameters { get; set; }
    }

    public class RootObject
    {
        public string ClassName { get; set; }
        public int StartNo { get; set; }
        public List<Function> Functions { get; set; }
    }
}
