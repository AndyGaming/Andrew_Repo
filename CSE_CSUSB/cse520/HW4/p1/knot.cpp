/*****************************************************************************

	knot.cpp
	    This program finds the knot vector ( u0, u1, ..., un-1 ) 
	of a B-spline. The program finds the knot vecotr by using the 
	steps provided in our lecture notes, then store each knot in 
	a vector. 

******************************************************************************/ 

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int cpoints, degree, i;

    cout << endl << "Please enter the number of control points: ";
    cin >> cpoints;
    cout << "Please enter the degree of spline: ";
    cin >> degree;

    int order = degree + 1;		//order is 1 less than degree
    int value = 1;
    int size = cpoints + order;		//number of knots = control points + order

    if (order > cpoints) {		//knot vector only exists when order <= control points
        cout << endl 
        << "Invalid input. Order should be less than control points. \n" << endl;
        return 0;
    }

    vector<int> kv;			//store each knot value

    //First m knots, u0, ..., um-1 all have value 0 
    for (i = 0; i < order; i++) 	
        kv.push_back(0);

    //Knots um, ..., un-1 increases in increments of value 1, from 1 to n - m
    for (i = order; i < cpoints; i++) 	
        kv.push_back(value++);

    //The final m knots, un, ..., un+m-1 are all equal to n - m + 1
    for (i = cpoints; i < size; i++)
        kv.push_back(value);

    cout << endl << "[Number of control points] " << cpoints; 
    cout << endl << "[Degree of spline] " << degree; 

    //print out the knot vector
    cout << endl << endl << "[Knot vector] \nU = ( ";
    for (i = 0; i < kv.size(); i++)
        cout << kv[i] << " ";
    cout << ")" << endl; 

    cout << endl; 
   
}
