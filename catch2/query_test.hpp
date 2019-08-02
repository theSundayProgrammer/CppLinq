#pragma once
struct pack
{
  int arr[2];
};


inline std::ostream& operator<<(std::ostream& stream, const pack& p)
{
  return stream << '(' << p.arr[0] << ", " << p.arr[1] << ')';
}

inline bool operator==(const pack& p1, const pack& p2)
{
  return p1.arr[0] == p2.arr[0] && p1.arr[1] == p2.arr[1];
}
inline bool operator!=(const pack& p1, const pack& p2) { return !(p1 == p2); }

