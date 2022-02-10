using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Controls;

namespace TetraSlide.Win8
{
    class SoundManager : Windows.UI.Xaml.DependencyObject
    {
        private MediaElement m_music;
        //private bool m_muted = false;
        private static SoundManager m_instance = null;
        public static SoundManager Instance()
        {
            if (m_instance == null)
                m_instance = new SoundManager();
            return m_instance;
        }
        public void StartMusic()
        {
            PlayMusic("Assets\\m1.mp3");
        }


        private async void LoadMedia(string path, Action<Windows.Storage.Streams.IRandomAccessStream, string> f)
        {
            var package = Windows.ApplicationModel.Package.Current;
            var installedLocation = package.InstalledLocation;
            var storageFile = await installedLocation.GetFileAsync(path);
            if (storageFile != null)
            {
                var stream = await storageFile.OpenAsync(Windows.Storage.FileAccessMode.Read);
                f(stream, storageFile.ContentType);
            }
        }
        private async void PlaySound(string path)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    LoadMedia(path, (stream, contentType) =>
                    {
                        var snd = new MediaElement();
                        snd.AudioCategory = AudioCategory.ForegroundOnlyMedia;
                        snd.SetSource(stream, contentType);
                    });
                });
        }
        private async void PlayMusic(string path)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    LoadMedia(path, (stream, contentType) =>
                    {
                        StopMusic();
                        m_music = new MediaElement();
                        m_music.AudioCategory = AudioCategory.BackgroundCapableMedia;
                        m_music.IsLooping = true;
                        m_music.SetSource(stream, contentType);
                    });
                });
        }
        private async void StopMusic()
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    if (m_music != null)
                        m_music.Stop();
                });
        }

        public void OnGameOver(object sender, GameOverEventArgs e)
        {
            StopMusic();
            if (e.Winner.Team == TsWinRT.TsPlayerTeam.TsTeamHome)
                PlaySound("Assets\\You win.wav");
            else
                PlaySound("Assets\\You lose.mp3");
            PlayMusic("Assets\\m1.mp3");
        }
        public void OnTetradFound(object sender, TetradFoundEventArgs e)
        {
            if (e.Player.Team == TsWinRT.TsPlayerTeam.TsTeamHome)
                PlaySound("Assets\\tetrad.mp3");
            else
                PlaySound("Assets\\hey.wav");
        }
        public void OnGameSuspended(object sender, GameSuspendedEventArgs e)
        {
            PlayMusic("Assets\\m1.mp3");
        }
        public void OnGameResumed(object sender, GameResumedEventArgs e)
        {
            PlayMusic("Assets\\ingame.mp3");
        }
    }
}
