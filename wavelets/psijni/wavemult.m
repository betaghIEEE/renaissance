
A = [64 128 128 128 128 128 128 128;
0 64 128 128 128 128 128 128;
0 0 64 128 128 128 128 128 ;
0 0 0 64 128 128 128 128;
0 0 0 0 64 128 128 128 ;
0 0 0 0 0 64 128 128;
0 0 0 0 0 0 64 128;
0 0 0 0 0 0 0 64 ];
A*A

[CA, CH, CV, CD] = dwt2(A, 'haar')
wA = [CA CV; CH CD];

[CA, CH, CV, CD] = dwt2(wA, 'haar')
wwA = [CA CV; CH CD];

[CA, CH, CV, CD] = dwt2(wwA, 'haar')
wwwA = [CA CV; CH CD];

wwwA
wwwA2 = wwwA*wwwA
CA = wwwA2(1:4,1:4);
CV = wwwA2(1:4,5:8);
CH = wwwA2(5:8,1:4);
CD = wwwA2(5:8,5:8);

wwA2 = idwt2(CA, CH, CV, CD, 'haar')
CA = wwA2(1:4,1:4);
CV = wwA2(1:4,5:8);
CH = wwA2(5:8,1:4);
CD = wwA2(5:8,5:8);

wA2 = idwt2(CA, CH, CV, CD, 'haar')
CA = wA2(1:4,1:4);
CV = wA2(1:4,5:8);
CH = wA2(5:8,1:4);
CD = wA2(5:8,5:8);

A2 = idwt2(CA, CH, CV, CD, 'haar')
A*A
sqrt(sum(sum( (A2-A*A).^2 ) ) / sum(sum((A*A).^2)) )
