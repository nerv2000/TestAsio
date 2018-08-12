using System;

namespace PacketGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            if (0 == args.Length)
            {
                Console.WriteLine("no parameter");
                return;
            }

            PacketGeneratorManager manager = new PacketGeneratorManager();
            if (false == manager.LoadPacketJsonFile(args[0]))
            {
                Console.WriteLine("file load fail");
                return;
            }
        }
    }
}
