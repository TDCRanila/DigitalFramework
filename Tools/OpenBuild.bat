cd ..
cd build
if exist ProjectSolution.sln (
	start ProjectSolution.sln
) else (
	start ProjectSolution.slnx
)