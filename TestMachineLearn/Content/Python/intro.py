import unreal

print("Hello ")

def ListAssetPaths():
    EAL = unreal.EditorAssetLibrary
    assetPaths = EAL.list_assets("/Game/ThirdPerson",True,False)
    for assetPath in assetPaths:
        print(assetPath)

ListAssetPaths()

