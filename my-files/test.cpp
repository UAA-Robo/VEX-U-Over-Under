// #include <iostream>
// #include <string>
// #include <future>
// #include <vector>

// using namespace std;

// int X_SIZE = 5;
// int Y_SIZE = 5;

// class Object
// {
// public:
//   int x;

//   Object(int x)
//   {
//     this->x = x;
//   }
// };

// vector<Object *> *assign(int num)
// {
//   vector<Object *> *arr;
//   arr = new vector<Object *>[Y_SIZE];
//   for (int y = 0; y < Y_SIZE; y++)
//   {
//     vector<Object *> arr[y];
//     arr[y].push_back(new Object(num));
//     cout << arr[y][0];
//   }
//   return arr;
// }

// int main()
// {
//   // Object ***array;
//   // int x_size = 5;
//   // int y_size = 5;

//   // array = new Object **[x_size];
//   // for (int i = 0; i < x_size; i++)
//   // {
//   //   array[i] = new Object *[y_size];
//   // }

//   // for (int x = 0; x < x_size; x++)
//   // {
//   //   for (int y = 0; y < y_size; y++)
//   //   {
//   //     array[x][y] = new Object(x + y);
//   //   }
//   // }

//   // for (int x = 0; x < x_size; x++)
//   // {
//   //   for (int y = 0; y < y_size; y++)
//   //   {
//   //     // cout << "X: " << x << ", Y: " << y << ", Element: " << array[x][y] << endl;
//   //     cout << "X: " << x << ", Y: " << y << ", Element: " << array[x][y]->x << endl;
//   //   }
//   // }

//   vector<Object *> ****array;
//   array = new vector<Object *> ***[X_SIZE];

//   for (int x = 0; x < X_SIZE; x++)
//   {
//     array[x] = new vector<Object *> **[Y_SIZE];
//     for (int y = 0; y < Y_SIZE; y++)
//     {
//       array[x][y] = new vector<Object *> *[X_SIZE];
//       for (int xf = 0; xf < X_SIZE; xf++)
//       {
//         array[x][y][xf] = new vector<Object *>[X_SIZE];
//         for (int yf = 0; yf < Y_SIZE; yf++)
//         {
//           // array[x][y][xf][yf] = new vector<Object *>;
//           // vector<Object *> array[x][y][xf][yf];
//         }
//       }
//     }
//   }

//   vector<future<vector<Object *> *>> futures;

//   for (int y = 0; y < Y_SIZE; y++)
//   {
//     futures.push_back(async(launch::async, assign, y));
//   }

//   for (int t = 0; t < Y_SIZE; t++)
//   {
//     // for (int y = 0; y < Y_SIZE; y++)
//     // {
//     // array[0][0][0][t] = futures[t].get();
//     // }
//     array[0][0][t] = futures[t].get();
//   }

//   // array[0][0][0][0].push_back(new Object(5));
//   // array[0][0][0][0].push_back(new Object(10));
//   // array[0][0][0][0].push_back(new Object(0));
//   // array[0][0][0][0].push_back(new Object(2));

//   for (int y = 0; y < Y_SIZE; y++)
//   {
//     cout << array[0][0][y][0][0]->x << endl;
//   }

//   // for (Object *obj : array[0][0][0][0])
//   // {
//   // cout << obj->x << endl;
//   // }
// }