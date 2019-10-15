# Games-Guy---2D
Example of a (probably not very good way!) to implement a few extra features with the awesome tutorials from That Games Guy


Original code and full credit to Robert Wells - http://thatgamesguy.co.uk/game-engine-dev/ as without those tutorials this file would not exist!

added: IMGUI - debug log
added: C_WarpLevelOnCollision - collider componenent - that makes use of a modification of the TileMapParser to read (badly) the object layer from tiled and set some level warps based on the tiled object properties (have a look at the NEW maps in tiled to see what I mean).

15/10/2019 
changes: moved from in scene level warp to new scenes per level! 

fix/change: I noticed the house exterior was missing some tiles on the left hand side, modified the code to make sure those were added and drawn (they had a -1 for their X position previously).

any questions, please ask - I am still a total noob so these additions and fixes are probably the worst way of doing things!! You have been warned, but maybe they will help someone?


LPC sprites GPL3 and CC-BY-SA3 - credit if you use any of them!

![level warp image](https://raw.githubusercontent.com/jponter/Games-Guy---2D/master/levelwarp.gif)
