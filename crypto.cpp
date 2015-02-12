#include <iostream>
#include <string>
#include <bitset>
#include <stdint.h>

using namespace std;
typedef uint32_t block;
typedef unsigned short int bit;

template <typename T>
string bin_repr(T value) {
    return bitset<sizeof(T)*8>(value).to_string();
}

block substitution (block b, block permu[]);

block S[16] = { 7, 3, 6, 1, 13, 9, 10, 11, 2, 12, 0, 4, 5, 15, 8, 14 };
block invS[16] = { 10, 3, 8, 1, 11, 12, 2, 0, 14, 5, 6, 7, 9, 4, 15, 13 };

// Matrix of linear approximations
block L[16][16];

bit scalarProduct (block a, block b)
{
    bit result = 0;

    block prod = a & b;
    block mask = 1;

    for (unsigned i = 0; i < 32; i++)
    {
	result = result ^ (mask & prod);
	prod = prod >> 1;
    }

    return result;
} 

void fillL ()
{
    for (block a = 0; a < 16; a++)
	for (block b = 0; b < 16; b++)
	    for (block x = 0; x < 16; x++)
	    {
		if (scalarProduct(a,x) == scalarProduct(b,substitution(x,S))) L[a][b]++;
	    }
}

// Encryption part
block substitution (block b, block permu[])
{
    block mask = 0xF;
    block shift = 0;
    block result = 0;

    for (unsigned i = 0; i < 8; i++)
    {
	block temp = b & mask;
	temp = temp >> shift;
	temp = permu[temp];
	temp = temp << shift;
	result += temp;
	shift += 4;
	mask = mask << 4;
    }

    return result;
}

block permutation (block b)
{
    block mask = 0b11;
    block result = b >> 2;
    mask = mask & b;
    mask = mask << 30;
    result += mask;

    return result;
}

block turn (block b, block key)
{

    block result = b;

    result = substitution(result, S);
    result = permutation(result);
    result = result ^ key;

    return result;
}

block encryption (block m, block k0, block k1, block k2)
{
    block result = m;

    result = result ^ k0;

    result = turn(result, k1);
    result = turn(result, k2);

    return result;
}

// Decryption part
block antiPermutation (block b)
{
    block mask = 0xC0000000;
    block result = b << 2;
    mask = mask & b;
    mask = mask >> 30;
    result += mask;

    return result;
}

block turnOver (block b, block key)
{

    block result = b;

    result = result ^ key;
    result = antiPermutation(result);
    result = substitution(result, invS);

    return result;
}

block decryption (block c, block k0, block k1, block k2)
{
    block result = c;

    result = turnOver(result, k2);
    result = turnOver(result, k1);

    result = result ^ k0;

    return result;
}

// Main
int main ()
{
    fillL();

    block m = 0;
    block k0 = 0x80000001;
    block k1 = 0xFFFFFFFF;
    block k2 = 0x7FFFFFFE;

    m = encryption(m,k0,k1,k2);
    cout << bin_repr(m) << endl;
    m = decryption(m,k0,k1,k2);
    cout << bin_repr(m) << endl;
 
   for (unsigned i = 0; i < 16; i++)
    {	
	for (unsigned j = 0; j < 16; j++)
	{
	    if (L[i][j] < 10)
		cout << L[i][j] << "  ";
	    else
		cout << L[i][j] << " ";
	}
	cout << endl;
    }
    return 0;
}
