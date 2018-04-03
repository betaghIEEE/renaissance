
B = 128*ones(8,8);
B2 = diag([64 64 64 64 64 64 64 64]);
B = B-B2

[CA, CH, CV, CD] = dwt2(B, 'haar')
wB = [CA CV; CH CD];

[CA, CH, CV, CD] = dwt2(wB, 'haar')
wwB = [CA CV; CH CD];

[CA, CH, CV, CD] = dwt2(wwB, 'haar')
wwwB = [CA CV; CH CD]

A = [64 128 128 128 128 128 128 128;
0 64 128 128 128 128 128 128;
0 0 64 128 128 128 128 128 ;
0 0 0 64 128 128 128 128;
0 0 0 0 64 128 128 128 ;
0 0 0 0 0 64 128 128;
0 0 0 0 0 0 64 128;
0 0 0 0 0 0 0 64 ]


[CA, CH, CV, CD] = dwt2(A, 'haar')
wA = [CA CV; CH CD];

[CA, CH, CV, CD] = dwt2(wA, 'haar')
wwA = [CA CV; CH CD];

[CA, CH, CV, CD] = dwt2(wwA, 'haar')
wwwA = [CA CV; CH CD]




wwwC2 = wwwA*wwwB
CA = wwwC2(1:4,1:4);
CV = wwwC2(1:4,5:8);
CH = wwwC2(5:8,1:4);
CD = wwwC2(5:8,5:8);

wwC2 = idwt2(CA, CH, CV, CD, 'haar')
CA = wwC2(1:4,1:4);
CV = wwC2(1:4,5:8);
CH = wwC2(5:8,1:4);
CD = wwC2(5:8,5:8);

wC2 = idwt2(CA, CH, CV, CD, 'haar')
CA = wC2(1:4,1:4);
CV = wC2(1:4,5:8);
CH = wC2(5:8,1:4);
CD = wC2(5:8,5:8);

C = idwt2(CA, CH, CV, CD, 'haar')
A*B
sqrt(sum(sum( (C-A*B).^2 ) ) / sum(sum((A*B).^2)) )
