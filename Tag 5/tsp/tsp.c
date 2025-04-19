#include "tsp.h"

int tsp_distanz(int perm[], int dist[], int len)
{
        int distanz = 0, i;
        for (i = 0; i < len - 1; i++)
        {
                distanz += dist[perm[i] * len + perm[i + 1]];  
        }
        distanz += dist[perm[len - 1] * len + perm[0]];
        return distanz;
}

void testopt(int perm[], int dist[], int opt[], int len)
{
        int distanz_opt, distanz_perm, i;
        distanz_perm = tsp_distanz(perm, dist, len);
        distanz_opt = tsp_distanz(opt, dist, len);
        if (distanz_perm < distanz_opt)
        {
                for (i = 0; i < len; i++)
                {
                        opt[i] = perm[i];
                }
                
        }
        
}

int next(int i, int perm[], int len)
{
        int j;
        for (j = i + 1; j < len; j++)
        {
                if (perm[j] == -1)
                {
                        return j;
                }
                
        }
        return len;
}

int minimum(int dist[], int len)
{
        int distanz = INT_MAX, i;
        for (i = 0; i < len * len; i++)
        {
                if (dist[i] != 0)
                {
                        if (distanz > dist[i])
                        {
                                distanz = dist[i];
                        }
                        
                }
                
        }
        return distanz;
}

int distanz_min(int perm[], int dist[], int len)
{
        int path_len = 0;
        int min = minimum(dist, len);
        int i;
        for (i = 0; i < len -1; i++)
        {
                if (perm[i] == -1 || perm[i + 1] == -1)
                {
                        path_len += min;
                        continue;
                }
                path_len += dist[perm[i] * len + perm[i + 1]];
        }
        return path_len;
}

void tsp_solve(int perm[], int opt[], int dist[], int ebene, int len)
{
        int i;
        if (ebene == len)
        {
                testopt(perm, dist, opt, len);
                return;
        }
        if (tsp_distanz(opt, dist, len) < distanz_min(perm, dist, len))
        {
                        return;
        }
        
        for (i = 0; i < len; i++)
        {
                if (perm[i] != -1)
                {
                        continue;
                }
                perm[i] = ebene;
                tsp_solve(perm, opt, dist, ebene +1, len);
                perm[i] = -1;
        }
        
}