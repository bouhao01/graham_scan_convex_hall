#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

typedef struct Point
{
    int x;
    int y;

    bool operator<(const Point &p) const
    {
        if (x < p.x)
        {
            return true;
        }
        else if (x == p.x)
        {
            return (y > p.y) ? true : false;
        }
        else
        {
            return false;
        }
    }
} point;

static point vect(const point &p, const point &q)
{
    Point res = {q.x - p.x, q.y - p.y};
    return res;
}

static int cross_product(const point &p, const point &q)
{
    return p.x * q.y - p.y * q.x;
};

vector<point> compute_hall(vector<point> input_points, bool is_upper = true)
{
    vector<point> output_points;
    stack<point> hall_stack;

    // sort point from left to right
    sort(input_points.begin(), input_points.end());

    hall_stack.push(input_points[0]);
    hall_stack.push(input_points[1]);

    point first_top, second_top;
    bool right_turn, wrong_turn;
    for (int i = 2; i < input_points.size(); ++i)
    {
        while (hall_stack.size() >= 2)
        {
            // Get the top two element in the stack
            first_top = hall_stack.top();
            hall_stack.pop();
            second_top = hall_stack.top();
            hall_stack.push(first_top);

            // Calculate the cross product to check whether it is a right turn or a left turn
            int corss_prod = cross_product(vect(second_top, first_top), vect(second_top, input_points[i]));
            if (0 == corss_prod)
            {
                // The three points are collinear
                wrong_turn = false;
            }
            else
            {
                right_turn = corss_prod < 0;
                wrong_turn = (is_upper) ? !right_turn : right_turn;
            }

            if (wrong_turn)
            {
                hall_stack.pop();
            }
            else
            {
                break;
            }
        }
        hall_stack.push(input_points[i]);
    }

    while (hall_stack.size() > 0)
    {
        point p = hall_stack.top();
        output_points.push_back(p);
        hall_stack.pop();
    }
    if (is_upper)
    {
        reverse(output_points.begin(), output_points.end());
    }

    return output_points;
}

int main()
{
    int n;
    vector<point> points, upper_convex_hall, lower_convex_hall, convex_hall;

    cin >> n;

    points.resize(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> points[i].x >> points[i].y;
    }

    // Compute the upper hall first
    upper_convex_hall = compute_hall(points);
    convex_hall = upper_convex_hall;

    if (convex_hall.size() != n)
    {
        // If all points are not collinear => compute the lower convex hall
        lower_convex_hall = compute_hall(points, false);
        for (int i = 1; i < lower_convex_hall.size() - 1; ++i)
        {
            convex_hall.push_back(lower_convex_hall[i]);
        }
    }

    // Add the last element: the same as the first element
    convex_hall.push_back(convex_hall.front());

    cout << endl<< "Convex hall (clockwise): " << endl;
    for (point p : convex_hall)
    {
        cout << p.x << " " << p.y << endl;
    }
    return 0;
}