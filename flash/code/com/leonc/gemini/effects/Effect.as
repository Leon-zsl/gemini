//特效对象

package com.scaleform.std.effects
{
    public class Effect 
    {
        public var value:Number = 0;
        public var destroyTime:Number = 0;
        public var tickSpeed:Number = 0;
        public var nextTickTime:Number = 0;
        
        public function Effect( value:Number, destroyTime:Number, tickSpeed:Number = 0 ) {
            this.value = value;
            this.destroyTime = destroyTime;
            this.tickSpeed = tickSpeed;
        }
    }

}