README - Making weapons thru bullets

When player creates bullets, bullets are rotated by 0 degrees
When enemy creates bullets, bullets are rotated by 180 degrees

The order of the parameters are fixed

There will be one bullet per line and has the following parameters:

#1
string spriteName, int health, int speed, bool invincible, float rotation

example:
bulletEnemy, 1, 10, false, 180.0f

#2 (the last parameter)
string spritename, int health, int speed, bool invincible,
string movementName, float spawnX, float spawnY, vector<float> params

example:
bulletEnemy, 1, 10, false, straight, enemy.x, enemy.y, {10.0f}