﻿package com.leonc.gemini.packet{	import flash.utils.ByteArray;	public class Packet	{		private var _opcode:int;		private var _buffer:ByteArray;				public function Packet()		{			_opcode = 0;			_buffer = null;		}				public function get opcode():int		{			return _opcode;		}				public function set opcode(val:int):void		{			_opcode = val;		}				public function get buffer():ByteArray		{			return _buffer;		}				public function set buffer(val:ByteArray):void		{			_buffer = val;			_buffer.position = 0;		}				public function get data():String		{			return _buffer.toString();		}				public function set data(val:String):void		{			_buffer = new ByteArray();			_buffer.writeMultiByte(val, "utf-8");			_buffer.position = 0;		}	}}