using System;
using System.IO;
using System.Text;
using PacketGenerator.JsonClass;
using Newtonsoft.Json;

namespace PacketGenerator
{
    class PacketGeneratorManager
    {
        private RootObject PacketData;

        public bool LoadPacketJsonFile(string path)
        {
            try
            {
                PacketData = JsonConvert.DeserializeObject<RootObject>
                    (
                        File.ReadAllText(path),
                        new JsonSerializerSettings
                        {
                            MissingMemberHandling = MissingMemberHandling.Error
                        }
                    );
            }
            catch (JsonSerializationException ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }

            CreatePacketFile();

            return true;
        }

        private string BlankString(int tapCount)
        {
            return String.Empty.PadLeft(4*tapCount, ' ');
        }

        private void CreatePacketFile()
        {
            CPlusPlus_CommonHeaderFile();
            CPlusPlus_CommonCppFile();

            CPlusPlus_RecvHeaderFile();
            CPlusPlus_RecvCppFile();

            CPlusPluse_SendHeaderFile();
            CPlusPluse_SendCppFile();
        }

        private void CPlusPlus_CommonHeaderFile()
        {
            FileInfo file = new FileInfo(string.Format("{0}_PacketCommon.h", PacketData.ClassName));
            FileStream fs = file.Create();
            TextWriter tw = new StreamWriter(fs, Encoding.ASCII);

            tw.WriteLine(BlankString(0) + "#pragma once");
            tw.WriteLine(BlankString(0) + "#include \"PacketStream.h\"");
            tw.WriteLine(BlankString(0) + "#include \"NetPacket.h\"");
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("class {0}_PacketCommon", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "public:");
            tw.WriteLine(BlankString(1) + string.Format("{0}_PacketCommon();", PacketData.ClassName));
            tw.WriteLine(BlankString(1) + string.Format("~{0}_PacketCommon();", PacketData.ClassName));
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + "protected:");
            tw.WriteLine(BlankString(1) + "enum PacketNo : uint16_t");
            tw.WriteLine(BlankString(1) + "{");

            bool isFirst = true;
            foreach (Function pf in PacketData.Functions)
            {
                if (true == isFirst)
                {
                    tw.WriteLine(BlankString(2) + string.Format("PN_{0} = {1}", pf.Name, PacketData.StartNo));
                    isFirst = false;
                }
                else
                {
                    tw.WriteLine(BlankString(2) + string.Format(", PN_{0}", pf.Name));
                }
            }

            tw.WriteLine(BlankString(1) + "};");
            tw.WriteLine();

            tw.WriteLine(BlankString(0) + "};");
            tw.WriteLine();

            tw.Close();
            fs.Close();
        }
        private void CPlusPlus_CommonCppFile()
        {
            FileInfo file = new FileInfo(string.Format("{0}_PacketCommon.cpp", PacketData.ClassName));
            FileStream fs = file.Create();
            TextWriter tw = new StreamWriter(fs, Encoding.ASCII);

            tw.WriteLine(BlankString(0) + string.Format("#include \"{0}_PacketCommon.h\"", PacketData.ClassName));
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("{0}_PacketCommon::{0}_PacketCommon()", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "}");
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("{0}_PacketCommon::~{0}_PacketCommon()", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "}");
            tw.WriteLine();

            tw.Close();
            fs.Close();
        }
        private void CPlusPlus_RecvHeaderFile()
        {
            FileInfo file = new FileInfo(string.Format("{0}_PacketRecv.h", PacketData.ClassName));
            FileStream fs = file.Create();
            TextWriter tw = new StreamWriter(fs, Encoding.ASCII);

            tw.WriteLine(BlankString(0) + "#pragma once");
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("class {0}_PacketRecv : public IRecvPacket, public {0}_PacketCommon", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "public:");
            tw.WriteLine(BlankString(0) + string.Format("{0}_PacketRecv();", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + string.Format("~{0}_PacketRecv();", PacketData.ClassName));
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + "private:");
            tw.WriteLine(BlankString(1) + "void CallPacketFunction(SessionID sessionId, PacketBufferPtr buffer) final;");
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + "public:");

            foreach (Function pf in PacketData.Functions)
            {
                string temp = string.Format("virtual void {0}(SessionID sessionId, ", pf.Name);
                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    temp += string.Format("{0}& {1}", pf.Parameters[i].Type, pf.Parameters[i].Name);
                    if (i != pf.Parameters.Count - 1)
                    {
                        temp += ", ";
                    }
                }
                temp += ") {}";
                tw.WriteLine(BlankString(1) + temp);
            }
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + "};");

            foreach (Function pf in PacketData.Functions)
            {
                string temp = string.Format("#define DECLARATION_{0} virtual void {0}(SessionID sessionId, ", pf.Name);
                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    temp += string.Format("{0}& {1}", pf.Parameters[i].Type, pf.Parameters[i].Name);
                    if (i != pf.Parameters.Count - 1)
                    {
                        temp += ", ";
                    }
                }
                temp += ") final";
                tw.WriteLine(BlankString(0) + temp);

                //#define DEFINITION_ReqTestPacket(className) void className##::ReqTestPacket(SessionID sessionId, int& data)
                string temp2 = string.Format("#define DEFINITION_{0}(className) void className##::{0}(SessionID sessionId, ", pf.Name);
                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    temp2 += string.Format("{0}& {1}", pf.Parameters[i].Type, pf.Parameters[i].Name);
                    if (i != pf.Parameters.Count - 1)
                    {
                        temp2 += ", ";
                    }
                }
                temp2 += ")";
                tw.WriteLine(BlankString(0) + temp2);
                tw.WriteLine();
            }

            tw.Close();
            fs.Close();
        }
        private void CPlusPlus_RecvCppFile()
        {
            FileInfo file = new FileInfo(string.Format("{0}_PacketRecv.cpp", PacketData.ClassName));
            FileStream fs = file.Create();
            TextWriter tw = new StreamWriter(fs, Encoding.ASCII);

            tw.WriteLine(BlankString(0) + string.Format("#include \"{0}_PacketRecv.h\"", PacketData.ClassName));
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("{0}_PacketRecv::{0}_PacketRecv()", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "}");
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("{0}_PacketRecv::~{0}_PacketRecv()", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "}");
            tw.WriteLine();

            tw.WriteLine(BlankString(0) + string.Format("void {0}_PacketRecv::CallPacketFunction(SessionID sessionId, PacketBufferPtr buffer)", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(1) + "PacketStream stream(buffer, PacketStream::READ);");
            tw.WriteLine(BlankString(1) + "PacketHeader* pHead = stream.GetPacketHead();");
            tw.WriteLine();
            tw.WriteLine(BlankString(1) + "switch (pHead->packetNo)");
            tw.WriteLine(BlankString(1) + "{");

            foreach (Function pf in PacketData.Functions)
            {
                tw.WriteLine(BlankString(2) + string.Format("case PN_{0} :", pf.Name));
                tw.WriteLine(BlankString(2) + "{");
                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    tw.WriteLine(BlankString(3) + string.Format("{0} {1};", pf.Parameters[i].Type, pf.Parameters[i].Name));
                }

                tw.WriteLine();

                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    tw.WriteLine(BlankString(3) + string.Format("stream >> {0};", pf.Parameters[i].Name));
                }

                tw.WriteLine();

                string temp = string.Format("{0}(sessionId, ", pf.Name);
                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    temp += string.Format("{0}", pf.Parameters[i].Name);
                    if (i != pf.Parameters.Count - 1)
                    {
                        temp += ", ";
                    }
                }
                temp += ");";
                tw.WriteLine(BlankString(3) + temp);

                tw.WriteLine(BlankString(2) + "}");
                tw.WriteLine(BlankString(2) + "break;");
            }
            tw.WriteLine(BlankString(2) + "default:");
            tw.WriteLine(BlankString(2) + "{");
            tw.WriteLine(BlankString(2) + "}");
            tw.WriteLine(BlankString(2) + "break;");
            tw.WriteLine(BlankString(1) + "}");

            tw.WriteLine(BlankString(0) + "}");

            tw.Close();
            fs.Close();
        }
        private void CPlusPluse_SendHeaderFile()
        {
            FileInfo file = new FileInfo(string.Format("{0}_PacketSend.h", PacketData.ClassName));
            FileStream fs = file.Create();
            TextWriter tw = new StreamWriter(fs, Encoding.ASCII);

            tw.WriteLine(BlankString(0) + "#pragma once");
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("class {0}_PacketSend : public ISendPacket, public {0}_PacketCommon", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "public:");
            tw.WriteLine(BlankString(1) + string.Format("{0}_PacketSend();", PacketData.ClassName));
            tw.WriteLine(BlankString(1) + string.Format("~{0}_PacketSend();", PacketData.ClassName));
            tw.WriteLine();
            foreach (Function pf in PacketData.Functions)
            {
                string temp = string.Format("void {0}(const SessionID sessionId, ", pf.Name);
                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    temp += string.Format("const {0}& {1}", pf.Parameters[i].Type, pf.Parameters[i].Name);
                    if (i != pf.Parameters.Count - 1)
                    {
                        temp += ", ";
                    }
                }
                temp += ");";
                tw.WriteLine(BlankString(1) + temp);
            }
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + "};");

            tw.Close();
            fs.Close();
        }
        private void CPlusPluse_SendCppFile()
        {
            FileInfo file = new FileInfo(string.Format("{0}_PacketSend.cpp", PacketData.ClassName));
            FileStream fs = file.Create();
            TextWriter tw = new StreamWriter(fs, Encoding.ASCII);

            tw.WriteLine(BlankString(0) + string.Format("#include \"{0}_PacketSend.h\"", PacketData.ClassName));
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("{0}_PacketSend::{0}_PacketSend()", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "}");
            tw.WriteLine();
            tw.WriteLine(BlankString(0) + string.Format("{0}_PacketSend::~{0}_PacketSend()", PacketData.ClassName));
            tw.WriteLine(BlankString(0) + "{");
            tw.WriteLine(BlankString(0) + "}");
            tw.WriteLine();

            foreach (Function pf in PacketData.Functions)
            {
                string temp = string.Format("void {0}_PacketSend::{1}(const SessionID sessionId, ", PacketData.ClassName, pf.Name);
                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    temp += string.Format("const {0}& {1}", pf.Parameters[i].Type, pf.Parameters[i].Name);
                    if (i != pf.Parameters.Count - 1)
                    {
                        temp += ", ";
                    }
                }
                temp += ")";
                tw.WriteLine(BlankString(0) + temp);
                tw.WriteLine(BlankString(0) + "{");
                tw.WriteLine(BlankString(1) + "PacketStream stream(std::make_shared<PacketBuffer>(), PacketStream::WRITE);");
                tw.WriteLine(BlankString(1) + "PacketHeader* pHead = stream.GetPacketHead();");
                tw.WriteLine();

                for (int i = 0; i < pf.Parameters.Count; ++i)
                {
                    tw.WriteLine(BlankString(1) + string.Format("stream << {0};", pf.Parameters[i].Name));
                }
                tw.WriteLine();

                tw.WriteLine(BlankString(1) + string.Format("pHead->packetNo = PN_{0};", pf.Name));
                tw.WriteLine(BlankString(1) + "pHead->size = stream.GetWriteLength();");
                tw.WriteLine();
                tw.WriteLine(BlankString(1) + "m_pPacketManager->SendNotify(sessionId, stream.GetBuffer());");

                tw.WriteLine(BlankString(0) + "}");
                tw.WriteLine();
            }

            tw.Close();
            fs.Close();
        }
    }
}
