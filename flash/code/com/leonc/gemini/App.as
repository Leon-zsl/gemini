package com.leonc.gemini {

	import flash.display.Stage;

	import com.leonc.gemini.system.Clock;
	import com.leonc.gemini.system.InputMgr;
	import com.leonc.gemini.system.SoundMgr;
	import com.leonc.gemini.packet.PacketDispatcher;
	import com.leonc.gemini.asset.AssetManager;
	import com.leonc.gemini.Game;

	public class App {
		private static var _instance:App;

		private var _root:Main;
		private var _frame:int;

		private var _clock:Clock = new Clock();
		private var _inputMgr = new InputMgr();
		private var _packetDispatcher:PackageDispatcher = new PackageDispatcher();
		private var _assetLoader:AssetManager = new AssetManager();
		private var _soundMgr = new SoundMgr();
		private var _game:Game = new Game();

		public function get instance():App { return _instance; }

		public function get root():Main { return _root; }
		public function get stage():Stage { return _root.stage; }
		public function get frame():int { return _frame; }

		public function get clock():Clock { return _clock; }
		public function get inputMgr():InputMgr { return _inputMgr; }
		public function get soundMgr():SoundMgr { return _soundMgr; }
		public function get packetDispathcer():PacketDispatcher	{ return _packetDispatcher;	}
		public function get assetLoader():AssetManager { return _assetLoader; }
		public function get game():Game { return _game; }

		public function startup(m:Main):void {
			_app = this;

			_root = m;
			_frame = 0;

			_clock.startup();
			_inputMgr.startup();
			_soundMgr.startup();
			_packetDispatcher.startup(st);
			_assetLoader.startup();
			_game.startup();
		}

		public function shutdown():void {
			_game.shutdown();
			_assetLoader.shutdown();
			_packetDispatcher.shutdown();
			_soundMgr.shutdown();
			_inputMgr.shutdown();
			_clock.shutdown();
		}

		public function update():void {
			_clock.update();
			_inputMgr.update();
			_soundMgr.update();
			_packetDispatcher.update();
			_assetLoader.update();
			_game.startup();

			_frame++;
		}
	}
}