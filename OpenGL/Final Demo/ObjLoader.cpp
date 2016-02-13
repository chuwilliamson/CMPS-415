#include "ObjLoader.h"

void LoadObj(char *fname,
	std::vector <v3> &v,
	std::vector <v3> &n,
	std::vector <v2> &uv,
	std::vector<unsigned int> &vi,
	std::vector<unsigned int> &uvi,
	std::vector<unsigned int> &ni)
{
	FILE *fp;
	v3 verts;
	v2 vert2;
	char line[1000];

	printf("Reading image file: %s\n", fname);

	fopen_s(&fp, fname, "r");
	int counter = 0;

	while (true)
	{
		int linePtr = fscanf_s(fp, "%s", line, sizeof(line));

		if (linePtr == EOF)
		{
			printf("done reading file \n");
			break;
		}

		if (strcmp(line, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f\n", &verts.x, &verts.y, &verts.z);
			v.push_back(verts);
		}
		else if (strcmp(line, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f\n", &verts.x, &verts.y, &verts.z);
			n.push_back(verts);
		}
		else if (strcmp(line, "vt") == 0)
		{
			fscanf_s(fp, "%f %f\n", &vert2.x, &vert2.y);
			uv.push_back(vert2);

		}
		//f v/vt/vn v/vt/vn v/vt/vn v/vt/vn
		//sometimes the line has 3 elements
		else if (strcmp(line, "f") == 0)
		{
			unsigned int t_vi[4], t_uv[4], t_ni[4];

			//								0		1		2			3
			int lineEle = fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&t_vi[0], &t_uv[0], &t_ni[0],
				&t_vi[1], &t_uv[1], &t_ni[1],
				&t_vi[2], &t_uv[2], &t_ni[2],//);
				&t_vi[3], &t_uv[3], &t_ni[3]);

			if (lineEle == 9)
			{
				counter++;

				vi.push_back(t_vi[0]);
				vi.push_back(t_vi[1]);
				vi.push_back(t_vi[2]);
				//vi.push_back(t_vi[3]);

				uvi.push_back(t_uv[0]);
				uvi.push_back(t_uv[1]);
				uvi.push_back(t_uv[2]);
				//uvi.push_back(t_uv[3]);

				ni.push_back(t_ni[0]);
				ni.push_back(t_ni[1]);
				ni.push_back(t_ni[2]);
				//ni.push_back(t_ni[3]);

			}
			else if (lineEle == 12)
			{

				vi.push_back(t_vi[0]);
				vi.push_back(t_vi[1]);
				vi.push_back(t_vi[2]);
				vi.push_back(t_vi[3]);

				uvi.push_back(t_uv[0]);
				uvi.push_back(t_uv[1]);
				uvi.push_back(t_uv[2]);
				uvi.push_back(t_uv[3]);

				ni.push_back(t_ni[0]);
				ni.push_back(t_ni[1]);
				ni.push_back(t_ni[2]);
				ni.push_back(t_ni[3]);

			}
		}
	}

}