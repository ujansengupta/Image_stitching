# Image_stitching

# Background
  One of the most intriguing problems in the domain of Computer Vision is to match and stitch two contiguous (or somehow related) scenes 
  or images. 
  If such a match is achieved, it implies that the computer is able to interpret and stitch images which, barring some overlap, are quite different. 
  This has wide ranging implications as it is a nascent yet significant step in equipping a machine with cognitive capabilities.

  Being able to stitch different images to construct a contiguous environment is an important aspect of many practical applications of Computer Vision. 
  From autonomous robot navigation and high-res photo mosaics to medical imaging and forensic science, image matching helps find patterns in different images 
  that may be inconspicuous to the human eye. 

  In the case of robot navigation, for example, stitching different images which are discrete for the robot (although humans understand them to be contiguous) helps 
  construct a map for the robot to navigate. Thus, making sure that the correct images are stitched to each other is vital in ensuring the correct understanding of the
  environment of the robot.

Although not aiming for something quite that ambitious, this project confines the scope to finding a match between two images with some overlap (around 40%). 
Those images are then stitched, providing us a continuous image with, hopefully, no aberrations.
The aim is to find a set of definite correspondence points between two images (since they’re both presumed to have at least 40% overlap) and to use those 
correspondence points to construct a homography matrix (referred to as the H matrix). Once the H matrix is found, simple matrix algebra can be applied to 
calculate all the correspondence points and then stitch the images.

The project is divided into two parts. The first part involves finding a certain number of correspondences manually and the second part involves finding the
correspondence points using an interest point detector and local area descriptors. Once the points are found, the H matrix is constructed and the images are 
stitched together by finding the correspondences for every pixel of either of the images to the other image.
