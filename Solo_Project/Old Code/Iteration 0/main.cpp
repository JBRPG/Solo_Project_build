/* Solo Project for CMPS 171
 * By John Peter McGrath
 */

#include <iostream>
#include "game.hpp"

using namespace std;


int main(){

	cout << "Solo Project in Progress" << endl;

	try {
	  Game game;
	  game.run();
	  return 0;
	} catch (runtime_error e) {
	  cout << e.what();
      return 1;
	}

}