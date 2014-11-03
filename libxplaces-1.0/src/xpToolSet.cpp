/*
 * xpToolSet.cpp
 *
 *  Created on: May 21, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpToolSet.h"
#include "xpPropertyList.h"

std::string xpToolSet::randomString(int length, bool letters, bool numbers, bool symbols) {
    std::string allPossible; // this will contain all necessary characters
    std::string str; // the random string
    if (letters == true) { // if you passed true for letters, we'll add letters to the possibilities
        for (int i = 65; i <= 90; i++) {
            allPossible += static_cast<char>(i);
            //allPossible += static_cast<char>(i+32); // add a lower case letter, too!
        }
    } if (numbers == true) { // if you wanted numbers, we'll add numbers
        for (int i = 48; i <= 57; i++) {
            allPossible += static_cast<char>(i);
        }
    } if (symbols == true) { // if you want symbols, we'll add symbols (note, their ASCII values are scattered)
        for (int i = 33; i <= 47; i++) {
            allPossible += static_cast<char>(i);
        } for (int i = 58; i <= 64; i++) {
            allPossible += static_cast<char>(i);
        } for (int i = 91; i <= 96; i++) {
            allPossible += static_cast<char>(i);
        } for (int i = 123; i <= 126; i++) {
            allPossible += static_cast<char>(i);
        }
    }
    // get the number of characters to use (used for rand())
    int numberOfPossibilities = allPossible.length();

#ifndef WIN32
    struct timeval now;

    long mtime;
    gettimeofday(&now, NULL);
    mtime = ((now.tv_sec) * 1000000 + now.tv_usec);


	srand (mtime);
	fprintf (stderr, "INIT SEED TO %ld\n", mtime);
#endif

    for (int i = 0; i < length; i++) {
        str += allPossible[rand() % numberOfPossibilities];
    }

    return str;
}


void xpToolSet::debugOutput(std::string l) {
	fprintf(stderr, "debug: %s\n",l.c_str());
}

void xpToolSet::debugOutput(xpPropertyList p) {
	for (unsigned int i = 0; i < p.size(); i++) {
		debugOutput(p[i].getKey() + " = " + p[i].getValue());
	}
}


