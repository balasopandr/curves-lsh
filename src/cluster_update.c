#include "math.h"
#include "cluster_update.h"
#include "complete_binary_tree.h"
#include "curve_metrics.h"

void MeanUpdate(Cluster *clusters, unsigned int num_of_clusters, void del_func(void *), double **dists)
{
	int i;
	for(i=0; i<num_of_clusters; i++)
	{
		int num_of_points = Cluster_get_points_count(clusters[i]);
		if(num_of_points == 0)
		{
			continue;
		}
		del_func(Cluster_get_medoid(clusters[i]));
		List leaves = Cluster_get_points(clusters[i]);
		unsigned int height = log(List_get_length(leaves))/log(2) + 1;

		CompleteBinaryTree cbt = CompleteBinaryTree_create(height, leaves);

		void *new_centre = PostOrderTraversal(cbt, dists);
		Cluster_set_medoid(clusters[i], new_centre, -1);
		CompleteBinaryTree_delete(cbt);
	}
}
