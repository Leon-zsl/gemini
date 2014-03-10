﻿package com.scaleform.std.packet{    import flash.events.Event;	public class PacketEvent extends Event	{        public static const CONN_ERROR:String = "conn_error";        private var _name:String;        private var _error:int;        private var _packet:Packet;        public function get name():String        {            return _name;        }        public function set name(val:String):void        {            _name = val;        }        public function get error():int        {            return _error;        }        public function set error(err:int):void        {            _error = err;        }        public function get packet():Packet        {            return _packet;        }        public function set packet(val:Packet):void        {            _packet = val;        }        public function PacketEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = true)        {            super( type, bubbles, cancelable );            _packet = null;        }                override public function clone():Event {            return new PacketEvent( type, bubbles, cancelable );        }                override public function toString():String {            return formatToString( "PacketEvent", "type", "bubbles", "cancelable" );        }	}}