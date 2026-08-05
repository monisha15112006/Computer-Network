#include <stdio.h>

#define MAX 10
#define INF 9999

int cost[MAX][MAX];
int dist[MAX];
int visited[MAX];
int nextHop[MAX];
int n;

void printTable(int source)
{
    int i;

    printf("\nConfirmed:\n");

    for(i=0;i<n;i++)
    {
        if(visited[i])
        {
            if(i==source)
                printf("(%c,0,-)\n",'A'+i);
            else
                printf("(%c,%d,%c)\n",'A'+i,dist[i],'A'+nextHop[i]);
        }
    }

    printf("\nTentative:\n");

    for(i=0;i<n;i++)
    {
        if(!visited[i])
        {
            if(dist[i]==INF)
                printf("(%c,INF,-)\n",'A'+i);
            else
                printf("(%c,%d,%c)\n",'A'+i,dist[i],'A'+nextHop[i]);
        }
    }
}

int main()
{
    int i,j;
    int choice,source;

    printf("Enter Number of Nodes: ");
    scanf("%d",&n);

    printf("Enter Cost Matrix:\n");

    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            scanf("%d",&cost[i][j]);

            if(cost[i][j]==0 && i!=j)
                cost[i][j]=INF;
        }
    }

    printf("\n1. Routing Table for All Nodes");
    printf("\n2. Routing Table for Particular Node");
    printf("\nEnter Choice: ");
    scanf("%d",&choice);
    if(choice==2)
    {
        printf("Enter Source Node (0-%d): ", n-1);
        scanf("%d",&source);

        /* Initialization */
        for(i=0;i<n;i++)
        {
            dist[i]=cost[source][i];
            visited[i]=0;

            if(cost[source][i]!=INF)
                nextHop[i]=i;
            else
                nextHop[i]=-1;
        }

        dist[source]=0;
        visited[source]=1;

        printf("\nInitial Routing Table\n");
        printTable(source);

        /* Dijkstra Algorithm */
        for(int count=1; count<n; count++)
        {
            int min=INF;
            int u=-1;

            for(i=0;i<n;i++)
            {
                if(!visited[i] && dist[i]<min)
                {
                    min=dist[i];
                    u=i;
                }
            }

            if(u==-1)
                break;

            visited[u]=1;

            for(j=0;j<n;j++)
            {
                if(!visited[j] && cost[u][j]!=INF)
                {
                    if(dist[u]+cost[u][j] < dist[j])
                    {
                        dist[j]=dist[u]+cost[u][j];

                        if(u==source)
                            nextHop[j]=j;
                        else
                            nextHop[j]=nextHop[u];
                    }
                }
            }

            printf("\nAfter Iteration %d\n",count);
            printTable(source);
        }

        printf("\nFinal Routing Table for Node %c\n",'A'+source);
        printf("--------------------------------------\n");
        printf("Destination\tCost\tNext Hop\n");

        for(i=0;i<n;i++)
        {
            printf("%c\t\t",'A'+i);

            if(dist[i]==INF)
                printf("INF\t-\n");
            else if(i==source)
                printf("%d\t-\n",dist[i]);
            else
                printf("%d\t%c\n",dist[i],'A'+nextHop[i]);
        }
    }    else if(choice==1)
    {
        int src;

        for(src=0; src<n; src++)
        {
            printf("\n====================================");
            printf("\nRouting Table for Node %c",'A'+src);
            printf("\n====================================\n");

            /* Initialization */
            for(i=0;i<n;i++)
            {
                dist[i]=cost[src][i];
                visited[i]=0;

                if(cost[src][i]!=INF)
                    nextHop[i]=i;
                else
                    nextHop[i]=-1;
            }

            dist[src]=0;
            visited[src]=1;

            printf("\nInitial Routing Table\n");
            printTable(src);

            /* Dijkstra Algorithm */
            for(int count=1; count<n; count++)
            {
                int min=INF;
                int u=-1;

                for(i=0;i<n;i++)
                {
                    if(!visited[i] && dist[i]<min)
                    {
                        min=dist[i];
                        u=i;
                    }
                }

                if(u==-1)
                    break;

                visited[u]=1;

                for(j=0;j<n;j++)
                {
                    if(!visited[j] && cost[u][j]!=INF)
                    {
                        if(dist[u]+cost[u][j] < dist[j])
                        {
                            dist[j]=dist[u]+cost[u][j];

                            if(u==src)
                                nextHop[j]=j;
                            else
                                nextHop[j]=nextHop[u];
                        }
                    }
                }

                printf("\nAfter Iteration %d\n",count);
                printTable(src);
            }

            printf("\nFinal Routing Table for Node %c\n",'A'+src);
            printf("--------------------------------------\n");
            printf("Destination\tCost\tNext Hop\n");

            for(i=0;i<n;i++)
            {
                printf("%c\t\t",'A'+i);

                if(dist[i]==INF)
                    printf("INF\t-\n");
                else if(i==src)
                    printf("%d\t-\n",dist[i]);
                else
                    printf("%d\t%c\n",dist[i],'A'+nextHop[i]);
            }
        }
    }
    else
    {
        printf("Invalid Choice\n");
    }

    return 0;
}
