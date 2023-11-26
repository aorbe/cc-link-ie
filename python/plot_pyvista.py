import pyvista as pv

# read the data
grid = pv.read('mesh_mcve.vtk')

# plot the data with an automatically created Plotter
grid.plot(show_scalar_bar=False, show_axes=False)


from vtkplotter import *

# read the data
ugrid = loadUnStructuredGrid("mesh_mcve.vtk")

# create the outline of the data as polygonal mesh and show it
Mesh(ugrid).c('viridis').alpha(1.0).show()