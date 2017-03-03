# BuildModel3D

This project is about get a mesh of an structure bone like a pelvis or femur in order to get data like a distance on important
areas for example distance between acetabulus and head femur in order to diagnostic an ill for example ostearthritis. the input
is a x-ray image from a patient and a mesh template with help us to adapt this template in the coords of the x-ray image.
So we will have a new mesh specific for the patient. (That's so cool).

The TODO list is the next:

1.- Obtain contour of the x-ray image in automatic form. (this can be obtain for manual segmentation by the doctor).
2.- Obtain the silhouette (contour) of the template mesh (It can be obtain in automatic form).
3.- Do registration work between both contour (It means adapt with rigid and non rigid registration) in 2D space
    with the projection of the template mesh 3D to 2D space)
4.- Apply the changes on the template mesh in 3D space with the reprojection. but considering the rest of the points of the mesh.
    (Laplace)
5.- You should always measure the distance point to point in 2d space between both contours in the registration process.
6.- Finally you get the most near mesh to the x-ray image and then measure the distance in the specific area on the mesh.
7.- If the distance is for above of the normal separation we can say the patient have signs of ostearthritis.
If the distance is very near to the normal separation we can say the patient have not problems of your hip bone.

What do we get with this experiment ?
1.- It could help to the patient and specialist to get more data of your evaluated part of body.
2.- It could help to understand if the patient have a possible ill of ostearthritis.
3.- the patient would have all information in x-ray image instead of obtain a very expensive Computed Tomography.
4.- And the most important is that the patient will not be hurt for the radiation exposure if he use a Computed Tomography.

