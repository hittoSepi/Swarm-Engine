#pragma once
#ifndef GameObject_MANAGER_
#define GameObject_MANAGER_

class   GameObjectManager: public Singleton<GameObjectManager>
    {
    public:
        friend class Singleton<GameObjectManager>;

        GameObjectManager();
        ~GameObjectManager();

        static void OnInit();
        static void OnUpdate();
        static void OnRender();
        static void BeforeQuit();

        static void Add(GameObject* obj);
        static void Remove(GameObject* obj);
        
        /// <summary>
        /// Create new <c> GameObject </c> and add it to GameObjectPool.
        /// Main way to create <c> GameObject </c> 
        /// </summary>
        /// <returns>Pointer to newly created <c> GameObject </c></returns>
        static GameObject* Create(std::string name); 

        inline static long long TotalGameObjectsCreated() { return gameObjectsCreated; }
    private:


        /// <summary>
        /// Adds new <c>GameObjects</c> from waiting queue to <c>GameObjectPool</c>
        /// </summary>
        static void AddObjects();

        /// <summary>
        /// Removes <c>GameObjects</c> from <c>GameObjectPool</c>
        /// </summary>
        static void RemoveObjects();

        static GameObjectPool*        GameObjects;
        static GameObjectList*        AddList;
        static HashList*              RemoveList;

        static GameObjectListIt       listIt;
        static GameObjectPoolIt       poolIt;

        // thread
        static void                    UpdateThread();
        inline static bool             Running = false;
        static std::thread*            updateThread;
        static std::mutex              lock;

        inline static long long        gameObjectsCreated = 0;
    };

#endif
