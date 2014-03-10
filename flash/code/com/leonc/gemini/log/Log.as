package com.leonc.gemini.log {
    import flash.external.*;
    public class Log {
        public static function Debug(info:String):void {
            ExternalInterface.call("Log_Debug", info);
        }
        public static function Info(info:String):void {
            ExternalInterface.call("Log_Info", info);
        }
        public static function Warn(info:String):void {
            ExternalInterface.call("Log_Warn", info);
        }
        public static function Error(info:String):void {
            ExternalInterface.call("Log_Error", info);
        }
    }
}