struct Body
{
	float2 pos;
	float2 r;
	float2 v;
	float2 f;
};

__constant const float gravity = 0.05;
__constant const float f_max = 1.0;
//__constant const float dt = 0.001;

__kernel void calcForces(__global struct Body* b)
{
	int t = 0;
	int i = get_global_id(0);
	int len = get_global_size(0);
	int j = 0;
	for(j; j < len; ++j)
	{ 
		if(i==j)
			continue;
		float dx = b[j].pos.x - b[i].pos.x;
		float dy = b[j].pos.y - b[i].pos.y;
		float r2 = 1.0/(dx*dx + dy*dy);
		float r1 = sqrt(r2);
		float f = min(gravity * b[i].r.x * b[j].r.x * r2, f_max);
		float tx = f * dx * r1;
		float ty = f * dy * r1;
		if(isnan(tx))
		{
			tx = 0.0;
		}		   
		if(isnan(ty))
		{
			ty = 0.0;
		}		 
		b[i].f.x = b[i].f.x + tx;
		b[i].f.y = b[i].f.y + ty;
		if(j==len-2)
		{ 
			t++;
		}
		//b[i].f = normalize(b[i].f);
	}	   
}

__kernel void move(__global struct Body* b, float dt)
{ 
	int i = get_global_id(0);
	float dvx = b[i].f.x * dt / b[i].r.x;
	float dvy = b[i].f.y * dt / b[i].r.x;
	b[i].pos.x += (b[i].v.x + dvx / 2) * dt;
	b[i].pos.y += (b[i].v.y + dvy / 2) * dt;
	b[i].v.x += dvx;
	b[i].v.y += dvy;
	b[i].f.x = 0;
	b[i].f.y = 0;
}
