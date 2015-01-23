Curve Project
===================
Curve Project is just a clone of Achtung die Kurve written in C++ and uses [SFML](https://github.com/LaurentGomila/SFML) so it is fast and simple. It was my first project in C++ and is therefore a big mess.

Build
-------
To build it look [here](http://sfml-dev.org/tutorials/2.2/) for info how to setup SFML.

To-Do List
----------------------------
* Clean up code
* Add version check
* Add AI
* Multiple players per client
    * Group clients in Server or add number denoting to which client the player belong
    * Set number of players when connecting (Maybe)
* Update id in player after client disconnect
* Add mutex to protect against simultaneous player addition
* Add mutex to protect access to socket when adding or deleting
* Add more powerups
    * Jumping line, line teleports forward instead of moving
* Fix variations in FPS
* Send linewidth to client(Sine powerup)
* Write a sf::VertexArray like class that supports erase();
* Use same code conventions everywhere
* Add checks to check incoming data
