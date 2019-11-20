#include <iostream>
#include "body_tracker.h"

template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
    if (pInterfaceToRelease != NULL)
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = NULL;
    }
}

/// <summary>
/// Constructor
/// </summary>
BodyTracker::BodyTracker() :
	m_nPreviousTime(0),
	m_pKinectSensor(NULL),
	m_pCoordinateMapper(NULL),
	m_pBodyFrameReader(NULL),
	m_pBodyIndexFrameReader(NULL),
	logDescription(true)
{
	bodyTexturePixels.assign(BodyTracker::cDepthWidth, std::vector<int>(BodyTracker::cDepthHeight));
}


/// <summary>
/// Destructor
/// </summary>
BodyTracker::~BodyTracker()
{
	// done with body index frame reader
	SafeRelease(m_pBodyIndexFrameReader);
    
	// done with body frame reader
	SafeRelease(m_pBodyFrameReader);
    
	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);
    
	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}
    
	SafeRelease(m_pKinectSensor);
}

void BodyTracker::Run()
{
    InitializeDefaultSensor();
}


void BodyTracker::Update(bool getMask)
{
    if ((!m_pBodyFrameReader) || (!m_pBodyIndexFrameReader))
    {
        return;
    }
    IBodyFrame* pBodyFrame = NULL;
    IBodyIndexFrame* pBodyIndexFrame = NULL;
	IFrameDescription* pBodyIndexFrameDescription = NULL;
    
    HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
	LOG(INFO) << "UPDATE";
		
    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;
    
        hr = pBodyFrame->get_RelativeTime(&nTime);
    
        IBody* ppBodies[BODY_COUNT] = { 0 };
    
        if (SUCCEEDED(hr))
        {
            hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
        }
    
        if (SUCCEEDED(hr))
        {
            ProcessBody(nTime, BODY_COUNT, ppBodies);
        }
    
        for (int i = 0; i < _countof(ppBodies); ++i)
        {
            SafeRelease(ppBodies[i]);
        }
    }
    
    if (getMask)
    {
        HRESULT hr = m_pBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame);
    
        if (SUCCEEDED(hr))
        {
			if (logDescription)
			{
				int width, height;
				unsigned bytesPerPixel, lengthInPixels;
				float diagonalFieldOfView, horizontalFieldOfView, verticalFieldOfView;

				pBodyIndexFrame->get_FrameDescription(&pBodyIndexFrameDescription);
				pBodyIndexFrameDescription->get_BytesPerPixel(&bytesPerPixel);
				pBodyIndexFrameDescription->get_DiagonalFieldOfView(&diagonalFieldOfView);
				pBodyIndexFrameDescription->get_HorizontalFieldOfView(&horizontalFieldOfView);
				pBodyIndexFrameDescription->get_VerticalFieldOfView(&verticalFieldOfView);
				pBodyIndexFrameDescription->get_Width(&width);
				pBodyIndexFrameDescription->get_Height(&height);
				pBodyIndexFrameDescription->get_LengthInPixels(&lengthInPixels);

				LOG(INFO) << "------ BODY INDEX FRAME DESCRIPTION -----";
				LOG(INFO) << "Frame size: " << width << " x " << height;
				LOG(INFO) << "Field of view (degrees): horizontal - " << horizontalFieldOfView
					<< ", vertical - " << verticalFieldOfView << ", diagonal - " << diagonalFieldOfView;
				LOG(INFO) << "Bytes per pixel: " << bytesPerPixel;
				LOG(INFO) << "Length in pixels: " << lengthInPixels;
				LOG(INFO) << "------ END BODY INDEX FRAME DESCRIPTION -----";

				logDescription = false;
			}

			byte* buffer;
			unsigned capacity;
			pBodyIndexFrame->AccessUnderlyingBuffer(&capacity, &buffer);

			LOG(INFO) << "UPDATE BODY MASK";

			const byte* bodyMask = const_cast<const byte*>(buffer);
			for (int i = 0; i < BodyTracker::cDepthWidth; i++)
			{
				for (int j = 0; j < BodyTracker::cDepthHeight; j++)
				{
					bodyTexturePixels[i][j] = bodyMask[i * BodyTracker::cDepthHeight + j];
				}
			}
        }
    }
    
    SafeRelease(pBodyFrame);
    SafeRelease(pBodyIndexFrame);
}


HRESULT BodyTracker::InitializeDefaultSensor()
{
    HRESULT hr;
    
    hr = GetDefaultKinectSensor(&m_pKinectSensor);
    if (FAILED(hr))
    {
        LOG(INFO) << "No ready Kinect found!";
        return E_FAIL;
    }

	LOG(INFO) << "------ START KINECT INITIALIZATION -----";
	LOG(INFO) << "Found Kinect";
    
    if (m_pKinectSensor)
    {
        // Initialize the Kinect and get coordinate mapper and the body reader
        IBodyFrameSource* pBodyFrameSource = NULL;
        IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;
    
        hr = m_pKinectSensor->Open();
    
        if (SUCCEEDED(hr))
        {
			LOG(INFO) << "Opened connection with Kinect";
            hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
        }
    
        if (SUCCEEDED(hr))
        {
			LOG(INFO) << "Initialized Coordinate Mapper";
            hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
        }
    
        if (SUCCEEDED(hr))
        {
			LOG(INFO) << "Initialized Body Frame Source";
            hr = m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
        }
    
        if (SUCCEEDED(hr))
        {
			LOG(INFO) << "Initialized Body Index Frame Source";
            hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
        }
    
        if (SUCCEEDED(hr))
        {
			LOG(INFO) << "Opened Body Frame Reader";
            hr = pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);
        }

		if (SUCCEEDED(hr))
		{
			LOG(INFO) << "Opened Body Index Frame Reader";
		}

		LOG(INFO) << "------ END KINECT INITIALIZATION -----";
    
        SafeRelease(pBodyFrameSource);
        SafeRelease(pBodyIndexFrameSource);
    }
    
    return hr;
}

void BodyTracker::ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies)
{
    HRESULT hr;
    
    if (m_pCoordinateMapper)
    {
        int width = 640;
        int height = 280;
        int nTrackedBodies = 0;
    
        // Calculate time delta from previous frame
        delta = static_cast<float>(nTime - m_nPreviousTime) / timeStampScale;
        m_nPreviousTime = nTime;
    
        left_idx = -1;
        right_idx = -1;
        for (int i = 0; i < nBodyCount; ++i)
        {
            IBody* pBody = ppBodies[i];
            massCenter[i] = sf::Vector2f(0.f, 0.f);
            if (pBody)
            {
                BOOLEAN bTracked = false;
                hr = pBody->get_IsTracked(&bTracked);
    
                if (bTracked)
                {
                    nTrackedBodies++;
    
                    Joint joints[JointType_Count];
    
                    hr = pBody->GetJoints(_countof(joints), joints);
    
                    if (SUCCEEDED(hr))
                    {
                        for (int j = 0; j < _countof(joints); ++j)
                        {
                            trackPointsXY[i][j] = GetProjection(BodyToScreen(joints[j].Position, width, height));
                            trackDepthPoint[i][j] = joints[j].Position.Z;
    
                            massCenter[i] = massCenter[i] + trackPointsXY[i][j];
                        }
                        massCenter[i] = massCenter[i] / static_cast<float>(_countof(joints));
    
                        for (int j = 0; j < static_cast<int>(Limbs::Type::Count); ++j)
                        {
                            sf::Vector2f newPointXY = LimbPointsXY(i, static_cast<Limbs::Type>(j));
                            limbVelocitiesXY[i][j] = (limbPointsXY[i][j] - newPointXY) / delta;
                            limbPointsXY[i][j] = newPointXY;
                            limbDepthPoint[i][j] = LimbDepthPoint(i, static_cast<Limbs::Type>(j));
    
                            /*LOG(INFO) << "Limb " << j << " velocity: (x: " << limbVelocitiesXY[i][j].x
                                << ", y:" << limbVelocitiesXY[i][j].y << ")";*/
                        }
                    }
    
                    if ((left_idx == -1) || (massCenter[i].x < massCenter[left_idx].x))
                    {
                        left_idx = i;
                    }
                    if ((right_idx == -1) || (massCenter[i].x > massCenter[right_idx].x))
                    {
                        right_idx = i;
                    }
                }
            }
        }
    
        if (left_idx == right_idx)
        {
            right_idx = -1;
        }
    
        LOG(INFO) << "Tracked bodies: n = " << nTrackedBodies
                << " - (left: " << left_idx << ", right: " << right_idx << ")";
    }
}


sf::Vector2f BodyTracker::BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height)
{
    // Calculate the body's position on the screen
    DepthSpacePoint depthPoint = { 0 };
    m_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);
    
    float screenPointX = static_cast<float>(depthPoint.X * width) / cDepthWidth;
    float screenPointY = static_cast<float>(depthPoint.Y * height) / cDepthHeight;
    
    return sf::Vector2f(screenPointX, screenPointY);
}


sf::Vector2f BodyTracker::getJointPointsXY(Joints::Type joint, bool left)
{
    int idx = (left) ? left_idx : right_idx;
    
    sf::Vector2f point;
    if (idx == -1)
    {
        point = sf::Vector2f(0, 0);
    }
    else
    {
        point = trackPointsXY[idx][static_cast<int>(joint)];
    }
    
    /*LOG(INFO) << "Joint " << static_cast<int>(joint) << " for player " << (left) << " - "
        << idx << "- (x: " << point.x << ", y:" << point.y << ")\n";*/
    
    return point;
}

float BodyTracker::getJointDepthPoint(Joints::Type joint, bool left)
{
    int idx = (left) ? left_idx : right_idx;
    if (idx == -1)
    {
        return 0.f;
    }
    return trackDepthPoint[idx][static_cast<int>(joint)];
}

sf::Vector2f BodyTracker::LimbPointsXY(int i, Limbs::Type limb)
{
    sf::Vector2f point(0.f, 0.f);
    
    if (i == -1)
    {
        return sf::Vector2f(0, 0);
    }
    
    //LOG(INFO) << "Limb " << static_cast<int>(limb) << " for player " << i << " - joints:";
    
    auto joints = limbJoints[limb];
    for (int j = 0; j < joints.size(); j++)
    {
        //LOG(INFO) << static_cast<int>(joints[j]);
        point += trackPointsXY[i][static_cast<int>(joints[j])];
    }
    point /= static_cast<float>(joints.size());
    
    //LOG(INFO) << "Limb position: (x: " << point.x << ", y:" << point.y << ")\n";
    
    return point;
}

float BodyTracker::LimbDepthPoint(int i, Limbs::Type limb)
{
    float point = 0.f;
    
    if (i == -1)
    {
        return 0.f;
    }
    
    //LOG(INFO) << "Limb " << static_cast<int>(limb) << " for player " << i << " - joints:";
    
    auto joints = limbJoints[limb];
    for (int j = 0; j < joints.size(); j++)
    {
        //LOG(INFO) << static_cast<int>(joints[j]);
        point += trackDepthPoint[i][static_cast<int>(joints[j])];
    }
    point /= static_cast<float>(joints.size());
    
    //LOG(INFO) << "Limb position: " << point;
    
    return point;
}

sf::Vector2f BodyTracker::GetProjection (const sf::Vector2f point)
{
    sf::Vector2f projection;
    
    projection.x = 1920 * (1 - point.x / cDepthWidth);
    projection.y = point.y * 1200 / cDepthHeight;
    
    return projection;
}

std::vector<std::vector<int>> & BodyTracker::getBodyMask()
{
    return bodyTexturePixels;
}

sf::Vector2f BodyTracker::getLimbPointsXY(Limbs::Type limb, bool left)
{
    int idx = (left) ? left_idx : right_idx;
    return limbPointsXY[idx][static_cast<int>(limb)];
}

float BodyTracker::getLimbDepthPoints(Limbs::Type limb, bool left)
{
    int idx = (left) ? left_idx : right_idx;
    return limbDepthPoint[idx][static_cast<int>(limb)];
}

sf::Vector2f BodyTracker::getLimbVelocitiesXY(Limbs::Type limb, bool left)
{
    int idx = (left) ? left_idx : right_idx;
    return limbVelocitiesXY[idx][static_cast<int>(limb)];
}