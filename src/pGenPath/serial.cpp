#include<iostream>
using namespace std;

int main()
{
    int num[100];
    int N;
    int i, j, tmp;

    //[input]
    cin >> N;
    for( i=0 ; i<N ; i=i+1 )
    {
        cin >> num[i];
    }

    //[sort]
    for( i=0 ; i<N ; i=i+1 )
    {
        for( j=i+1 ; j<N ; j=j+1 )
        {
            if( num[i] > num[j] )
            {
                //變數交換
                tmp = num[i];
                num[i] = num[j];
                num[j] = tmp;
            }
        }
    }

    //[output]
    for( i=0 ; i<N ; i=i+1 )
    {
        cout << num[i] << " ";
    }
    cout << endl;

    return 0;
}
